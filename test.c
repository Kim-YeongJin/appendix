// 201424528-Jeong-SeungGyu.c -- Take home midterm, Spring 2020.
// Jeong Seung Gyu, id #201424528

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define BUFFSIZE 1024		// Message size
#define MAXACCOUNT 10		// Maximum number of user
#define ID_LENGTH 20		// Length of ID

typedef struct message_buffer {	// define struct for chatting message

    char msg[BUFFSIZE];				// content of messages
    char userID[ID_LENGTH];			// who send the messages
    int message_id;				// ID of messages
    int countMessage;				// the number of messages	
    char account[MAXACCOUNT][ID_LENGTH];	// store userIDs
    int countAccount;				// the number of users
    int lastExit;				// stored_index of last exit

} msg_buff;

typedef struct {

    char msg[BUFFSIZE];				// content of messages
    char userID[ID_LENGTH];			// who send the messages
    int message_id;				// ID of messages

} fetch_msg;

fetch_msg *newMsg;		// It will save the fetched message

WINDOW *input_screen;		// input screen
WINDOW *chat_screen;		// chatting screen
WINDOW *time_screen;		// time screen
WINDOW *account_screen;		// user account screen

msg_buff *buff_in = NULL;	// Buffer to store messages
msg_buff *buff_out = NULL;	// Buffer to read messages

int is_running;			// Variable that identifies if the chat is running
int current_account = 0;	// Index that userid stored in account [] [] of shared memory
int stored_index;		// Variable identifying the account's userID
				// in the account[][] of msg_buff structure

int shmid;			// shared memory ID
char userID[20];		// user name
char inputstr[40];		// input message

struct shmid_ds shm_info;	// store shared memory's state
void *shmaddr = (void*)0;	// address of shared memory
int recentReadTime = 0;		// The last message number read by the process

sem_t *mySem;			// Semaphore to control access to shared memory

pthread_mutex_t *mut;		// Mutex to control access to fetched messages
pthread_cond_t *cond;		// Access control with mutex
int newFetch = 0;		// If new message is fetched, newFetch turns to 1
int ret_count;			// Return value of mutex control function

// prototype of functions
void *get_input();
void *print_time();
void *print_account();
void *auto_chat();
void *FetchMessageFromShmThread();
void *DisplayMessageThread();
void chat();
void die(char *msg);
void chat_remove(int shmid);

int main(int argc, char* argv[])
{
    // If a process is executed without a user ID, an error message is output.
    if(argc < 2){
	fprintf(stderr, "[Usage]: ./chat UserID \n");
	exit(-1);
    }
    // copy userID from parameter to char userID[20]
    strcpy(userID, argv[1]);
    sem_unlink("mySem");
    mySem = (sem_t *)malloc(sizeof(sem_t));

    // If shared memory is not yet created, create it.
    shmid = shmget((key_t)20200406, sizeof(msg_buff) * 100, 0666|IPC_CREAT|IPC_EXCL);
    
    // If it is created, attach it by receiving the shared memory ID
    if(shmid < 0){
	shmid = shmget((key_t)20200406, sizeof(msg_buff) * 100, 0666);
	shmaddr = shmat(shmid, (void *)0, 0666);
	
	// If attach fails, exit        
	if(shmaddr == (void *)-1){
	    perror("shmat attach is failed: ");
	    exit(-1);
        }
    }
    else{
	// The process that created the shared memory also attaches it.
	shmaddr = shmat(shmid, (void *)0, 0666);
    }    

    // Allocate shared memory addresses to buff_in and buff_out
    buff_in = (msg_buff *) shmaddr;
    buff_out = (msg_buff *) shmaddr;

    // Dynamically allocate the message structure to be fetched
    newMsg = (fetch_msg *)malloc(sizeof(fetch_msg));

    // Create a named semaphore.
    if(buff_in->countAccount == 0)
    {
        mySem = sem_open("mySem", O_CREAT|O_EXCL, 0666, 1);
    }
    else
    {
        mySem = sem_open("mySem", O_CREAT, 0666, 1);
    }

    // If the creation of the named semaphore fails, an error is returned
    if(mySem == SEM_FAILED)
    {
	chat_remove(shmid);
	perror("sem_open [mySem] : ");
	exit(-1);
    }

    // Initialize by dynamically allocating mutex and cond
    mut = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    ret_count = pthread_mutex_init(mut, NULL);
    if(ret_count)
    {
	fprintf(stderr, "ERROR: Return code from pthread_mutex_init() is %d\n", ret_count);
	exit(-1);
    }
    cond = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(cond, NULL);

    // Display the ncurses screen and start chatting
    initscr();
    chat();

    // When the chat is finished, deallocate the memory of the mutex, cond and newMsg structures
    ret_count = pthread_mutex_destroy(mut);
    if(ret_count)
    {
	fprintf(stderr, "ERROR: Return code from pthread_mutex_destroy() is %d\n", ret_count);
	exit(-1);
    }
    free(mut);

    pthread_cond_destroy(cond);
    free(cond);
    free(newMsg);

    // If no process is accessing the shared memory after the chat is finished, execute chatremove
    shmctl(shmid, IPC_STAT, &shm_info);
    if(shm_info.shm_nattch == 0){
	printf("chat remove\n");
	chat_remove(shmid);
	// Remove named semaphore
	sem_unlink("mySem");
    }

    return 0;
}

void chat()
{
    // Specifies the size of each screen
    input_screen = newwin(4, 60, 20, 0);
    chat_screen = newwin(20, 60, 0, 0);
    time_screen = newwin(4, 20, 20, 60);
    account_screen = newwin(20, 20, 0, 60);

    // disappear cursor
    curs_set(0);

    mvwprintw(chat_screen, 1, 15, "***** Type /exit to quit!! *****\n");
    mvwprintw(account_screen, 1, 1, "*** user list ***\n");

    // Store user ID in account [] [] of shared memory 
    // and stored_index is set to the current number of users
    sprintf(buff_in->account[buff_in->countAccount], "%s\n", userID);
    stored_index = buff_in->countAccount;
    buff_in->countAccount++;
    recentReadTime = buff_in->countMessage;

    // Specifies the border of each screen
    box(input_screen, ACS_VLINE, ACS_HLINE);
    box(chat_screen, ACS_VLINE, ACS_HLINE);
    box(time_screen, ACS_VLINE, ACS_HLINE);
    box(account_screen, ACS_VLINE, ACS_HLINE);

    // Set the chat screen to be scrollable
    scrollok(chat_screen, TRUE);

    // Refresh all screens
    wrefresh(input_screen);
    wrefresh(chat_screen);
    wrefresh(time_screen);
    wrefresh(account_screen);

    // The message_id of buff_in and buff_out is initialized to 0 
    // and is_running is set to 1 so that the chat starts
    buff_in->message_id = 0;
    buff_out->message_id = 0;
    is_running = 1;

    // Execute each function as a thread
    pthread_t thread[6];

    pthread_create(&thread[0], NULL, get_input, NULL);
    pthread_create(&thread[1], NULL, print_time, NULL);
    pthread_create(&thread[2], NULL, print_account, NULL);
    pthread_create(&thread[3], NULL, auto_chat, NULL);
    pthread_create(&thread[4], NULL, FetchMessageFromShmThread, NULL);
    pthread_create(&thread[5], NULL, DisplayMessageThread, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);
    pthread_join(thread[3], NULL);
    pthread_join(thread[4], NULL);
    pthread_join(thread[5], NULL);

    die("bye");
}

void *get_input()
{
    char tmp[BUFFSIZE];

    while(is_running)
    {
	// get a string from the input screen
	mvwgetstr(input_screen, 1, 1, tmp);
	
	// if string is "/bye"
	if(strcmp(tmp, "/bye") == 0)
	{
	    // Wait for semaphore before accessing shared memory
	    sem_wait(mySem);
	    // The IDs stored after the exiting user ID are moved forward one by one
	    if(stored_index != buff_in->countAccount)
	    {
	        for(int i=stored_index; i<buff_in->countAccount; i++)
	        {
		    sprintf(buff_in->account[i], "%s\n", "");
		    sprintf(buff_in->account[i], "%s\n", buff_in->account[i + 1]);
	        }
	    }
	    else
	    {
		sprintf(buff_in->account[stored_index], "%s\n", "");
	    }
	    // Make countAccount -1 
	    // And change is_running to 0 to end the chat
	    buff_in->lastExit = stored_index;
	    buff_in->countAccount--;

	    // Unlock semaphore when job is finished
	    sem_post(mySem);
	    is_running = 0;

	}

	// string is not "/bye"
	else
	{
	    // Wait for semaphore before accessing shared memory
	    sem_wait(mySem);
	    // Store the message in shared memory
	    sprintf(buff_in[buff_in->countMessage].msg, "%s\n", tmp);
	    sprintf(buff_in[buff_in->countMessage].userID, "%s", userID);
	    buff_in[buff_in->countMessage].message_id = buff_in->countMessage + 1;
	    buff_in->countMessage++;
	    // Wait for semaphore before accessing shared memory
	    // Unlock semaphore when job is finished
	    sem_post(mySem);

	    // Update screens
    	    box(chat_screen, ACS_VLINE, ACS_HLINE);
	    wrefresh(chat_screen);
	    werase(input_screen);
	    box(input_screen, ACS_VLINE, ACS_HLINE);
	    wrefresh(input_screen);
	}

	usleep(100);
    }

    return NULL;
}

void *print_time()
{
    // Variables to store current hours, minutes and seconds
    int hour, minute, second;
    // Variable that stores the time the program was run and elapsed
    int elapsed = 0;
    int timecount = 0;

    // Type and struct used to express time included in time.h
    time_t now; 
    struct tm ts;
    // Variable to store the converted time string
    char current_time[80];

    while(is_running){


	// calculate hours, minutes, seconds individually.
	second = elapsed % 60;
	minute = (elapsed / 60) % 60;
	hour = elapsed / 3600;

        // Calculate time and convert it to the desired form.
	time(&now);
	ts = *localtime(&now);
	strftime(current_time, sizeof(current_time), "%H:%M:%S", &ts);

	// Output the result to the time screen.
	mvwprintw(time_screen, 1, 5, current_time);  
        mvwprintw(time_screen, 2, 5, "%02d:%02d:%02d", hour, minute, second);

	wrefresh(time_screen);
	// Sleep for 0.5 second and Increase elapsed by 1 every second.	
	usleep(500000);
	timecount++;
	if(timecount % 2 == 0)
	{
	    elapsed++;
	}
    }

    return NULL;
}

void *print_account()
{
    while(is_running)
    {
	// If the number of users participating in the chat changes
	if(buff_in->countAccount != current_account)
	{
	    werase(account_screen);
	    mvwprintw(account_screen, 1, 1, "*** user list ***\n");

	    // Re-read and print user IDs from shared memory
	    for(int i=0; i<buff_in->countAccount; i++)
	    {
		mvwprintw(account_screen, i + 2, 1, "%s", buff_in->account[i]);
	    }

	    // Update stored_index
	    if(buff_in->countAccount < current_account 
		&& buff_in->lastExit < stored_index) 
	    {
		stored_index -= 1;
	    }

	    // Refresh account screen
	    box(account_screen, ACS_VLINE, ACS_HLINE);
	    wrefresh(account_screen);
	    current_account = buff_in->countAccount;
	}
    }

    return NULL;
}

void *auto_chat()
{
    char msg[BUFFSIZE];
    int auto_chat_count = 0;
    srand(time(NULL));		// Set the seed to use a random function
    int t;

    while(is_running)
    {
	t = (rand() % 1001) + 1000;
	if(strcmp(userID, "Jico") == 0)
	{
	    // Wait for semaphore before accessing shared memory
	    sem_wait(mySem);

	    // Stores automatic messages in shared memory
	    strcpy(msg, "My name is Jico I love to sing any song-");
	    sprintf(buff_in[buff_in->countMessage].msg, "%s%d\n", msg, ++auto_chat_count);
	    sprintf(buff_in[buff_in->countMessage].userID, "%s", userID);
	    buff_in[buff_in->countMessage].message_id = buff_in->countMessage + 1;
	    buff_in->countMessage++;

	    // Unlock semaphore when job is finished
	    sem_post(mySem);

	}
	else if(strcmp(userID, "lzzy") == 0)
	{
	    // Wait for semaphore before accessing shared memory
	    sem_wait(mySem);

	    // Stores automatic messages in shared memory
	    strcpy(msg, "I am Issy I like to play on the stage. Ho-");
	    sprintf(buff_in[buff_in->countMessage].msg, "%s%d\n", msg, ++auto_chat_count);
	    sprintf(buff_in[buff_in->countMessage].userID, "%s", userID);
	    buff_in[buff_in->countMessage].message_id = buff_in->countMessage + 1;
	    buff_in->countMessage++;

	    // Unlock semaphore when job is finished
	    sem_post(mySem);
	}
	// sleep randomly between 1 sec and 2secs
	usleep(t * 1000);
    }

    return NULL;
}

void *FetchMessageFromShmThread()
{
    while(is_running)
    {
	if(recentReadTime != buff_out->countMessage)
	{
	    // Implement mutex lock for sequential execution of threads
	    pthread_mutex_lock(mut);

	    // fetch message from shared memory
	    // Wait for semaphore before accessing shared memory
	    sem_wait(mySem);
	    strcpy(newMsg->msg, buff_out[recentReadTime].msg);
	    strcpy(newMsg->userID, buff_out[recentReadTime].userID);
	    newMsg->message_id = buff_out->countMessage;
	    // Unlock semaphore when job is finished
	    sem_post(mySem);

	    // Wakes up DisplayMessageThread
	    newFetch = 1;
	    pthread_cond_signal(cond);

	    // Unlock mutex when job is finished
	    pthread_mutex_unlock(mut);
	}
    }

    return NULL;
}

void *DisplayMessageThread()
{
    while(is_running)
    {
	if(newFetch)
	{
	    // Implement mutex lock for sequential execution of threads
	    pthread_mutex_lock(mut);
	    // Wait for signal to be sent to cond
	    pthread_cond_wait(cond, mut);

	    // Wait for semaphore before accessing shared memory
	    sem_wait(mySem);
	    // Read and print from fetched message
	    wprintw(chat_screen, "  [%s: %d] > %s", 
		newMsg->userID,
		newMsg->message_id,
		newMsg->msg);
	    recentReadTime++;
	    // Unlock semaphore when job is finished
	    sem_post(mySem);

	    // Refresh screen
	    wrefresh(input_screen);
	    box(chat_screen, ACS_VLINE, ACS_HLINE);
	    wrefresh(chat_screen);

	    // Unlock mutex when job is finished
	    pthread_mutex_unlock(mut);
	    newFetch = 0;
	    usleep(100);
	}
    }

    return NULL;
}

void die(char *s)
{  
    usleep(100);
    shmdt(shmaddr);
    delwin(input_screen);
    delwin(chat_screen);
    delwin(account_screen);
    delwin(time_screen);
    endwin();
    perror(s);
}

// get the shared memory ID to be deleted as a parameter
void chat_remove(int shmid){

    // if shmid < 0, an error has occurred and terminate.
    if(shmid < 0){
        perror("shmget failed: ");
	exit(-1);
    }

    // Remove shared memory 
    // and exit if an error occurs during this process
    if(shmctl(shmid, IPC_RMID, 0) < 0){
	printf("Failed to delete shared memory\n");
	exit(-1);
    }
    // If everything went well, quit.
    else{
	printf("Successfully delete shared memory\n");
    }

    return;
}










