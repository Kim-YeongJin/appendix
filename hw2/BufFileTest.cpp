#include <fstream>
#include <iomanip>
#include <cstring>
#include "iobuffer.h"
#include "fixfld.h"
#include "length.h"
#include "student.h"
#include "coursereg.h"
#include "buffile.h"

student st[10];
coursereg cr[10];

void InitPerson()
{
	cout << "Initializing 10 Students"<<endl;
    strcpy(st[0].identifier, "10");
    strcpy(st[0].name, "A");
    strcpy(st[0].Address, "Seoul");
    strcpy(st[0].dofe, "2014");
    strcpy(st[0].credit_hours, "10");
    st[0].Print (cout);
    strcpy(st[1].identifier, "11");
    strcpy(st[1].name, "B");
    strcpy(st[1].Address, "Seoul");
    strcpy(st[1].dofe, "2014");
    strcpy(st[1].credit_hours, "10");
    st[1].Print (cout);
    strcpy(st[2].identifier, "12");
    strcpy(st[2].name, "C");
    strcpy(st[2].Address, "Seoul");
    strcpy(st[2].dofe, "2014");
    strcpy(st[2].credit_hours, "10");
    st[2].Print (cout);
    strcpy(st[3].identifier, "13");
    strcpy(st[3].name, "D");
    strcpy(st[3].Address, "Seoul");
    strcpy(st[3].dofe, "2014");
    strcpy(st[3].credit_hours, "10");
    st[3].Print (cout);
    strcpy(st[4].identifier, "14");
    strcpy(st[4].name, "E");
    strcpy(st[4].Address, "Seoul");
    strcpy(st[4].dofe, "2014");
    strcpy(st[4].credit_hours, "10");
    st[4].Print (cout);
    strcpy(st[5].identifier, "15");
    strcpy(st[5].name, "F");
    strcpy(st[5].Address, "Seoul");
    strcpy(st[5].dofe, "2014");
    strcpy(st[5].credit_hours, "10");
    st[5].Print (cout);
    strcpy(st[6].identifier, "16");
    strcpy(st[6].name, "G");
    strcpy(st[6].Address, "Seoul");
    strcpy(st[6].dofe, "2014");
    strcpy(st[6].credit_hours, "10");
    st[6].Print (cout);
    strcpy(st[7].identifier, "17");
    strcpy(st[7].name, "H");
    strcpy(st[7].Address, "Seoul");
    strcpy(st[7].dofe, "2014");
    strcpy(st[7].credit_hours, "10");
    st[7].Print (cout);
    strcpy(st[8].identifier, "18");
    strcpy(st[8].name, "I");
    strcpy(st[8].Address, "Seoul");
    strcpy(st[8].dofe, "2014");
    strcpy(st[8].credit_hours, "10");
    st[8].Print (cout);
    strcpy(st[9].identifier, "19");
    strcpy(st[9].name, "J");
    strcpy(st[9].Address, "Seoul");
    strcpy(st[9].dofe, "2014");
    strcpy(st[9].credit_hours, "10");
    st[9].Print (cout);
    
    cout << "Initializing 10 Course Registration"<<endl;
    strcpy(cr[0].course_id, "1");
    strcpy(cr[0].student_id, "a");
    strcpy(cr[0].course_credit_hours, "10");
    strcpy(cr[0].course_grade, "A");
    cr[0].Print (cout);
    strcpy(cr[1].course_id, "2");
    strcpy(cr[1].student_id, "b");
    strcpy(cr[1].course_credit_hours, "10");
    strcpy(cr[1].course_grade, "A");
    cr[1].Print (cout);
    strcpy(cr[2].course_id, "3");
    strcpy(cr[2].student_id, "c");
    strcpy(cr[2].course_credit_hours, "10");
    strcpy(cr[2].course_grade, "A");
    cr[2].Print (cout);
    strcpy(cr[3].course_id, "4");
    strcpy(cr[3].student_id, "d");
    strcpy(cr[3].course_credit_hours, "10");
    strcpy(cr[3].course_grade, "A");
    cr[3].Print (cout);
    strcpy(cr[4].course_id, "5");
    strcpy(cr[4].student_id, "e");
    strcpy(cr[4].course_credit_hours, "10");
    strcpy(cr[4].course_grade, "A");
    cr[4].Print (cout);
    strcpy(cr[5].course_id, "6");
    strcpy(cr[5].student_id, "f");
    strcpy(cr[5].course_credit_hours, "10");
    strcpy(cr[5].course_grade, "A");
    cr[5].Print (cout);
    strcpy(cr[6].course_id, "7");
    strcpy(cr[6].student_id, "g");
    strcpy(cr[6].course_credit_hours, "10");
    strcpy(cr[6].course_grade, "A");
    cr[6].Print (cout);
    strcpy(cr[7].course_id, "8");
    strcpy(cr[7].student_id, "h");
    strcpy(cr[7].course_credit_hours, "10");
    strcpy(cr[7].course_grade, "A");
    cr[7].Print (cout);
    strcpy(cr[8].course_id, "9");
    strcpy(cr[8].student_id, "i");
    strcpy(cr[8].course_credit_hours, "10");
    strcpy(cr[8].course_grade, "A");
    cr[8].Print (cout);
    strcpy(cr[9].course_id, "10");
    strcpy(cr[9].student_id, "j");
    strcpy(cr[9].course_credit_hours, "10");
    strcpy(cr[9].course_grade, "A");
    cr[9].Print (cout);
    
    
}


void testBufferFile (IOBuffer & Buff, char * myfile)
{
	student stu;
    coursereg cou;
	int result;
    
    int st_recaddr[10];
    int cr_recaddr[10];

	// Test writing
    BufferFile TestOut (Buff);
    // Note that header information is also written
    result = TestOut.Create (myfile, ios::in|ios::out);
    cout << "create file "<<result<<endl;
    if (!result)
    {
        cout << "Unable to create file "<<myfile<<endl;
        return;
    }
   
    for(int i=0; i<10; i++){
	st[i].Pack (TestOut.GetBuffer());
	st_recaddr[i] = TestOut.Write();
	cout << "<student> write at " << st_recaddr[i] << endl;
    }

    for(int i=0; i<10; i++){
	cr[i].Pack (TestOut.GetBuffer());
        cr_recaddr[i] = TestOut.Write();	
	cout << "<course> write at " << cr_recaddr[i] << endl;
    }
    
    TestOut.Close ();

	// test reading
    BufferFile TestIn (Buff);
    TestIn.Open (myfile, ios::in);
    
    for(int i=0; i<10; i++){
	TestIn.Read(st_recaddr[i]);
        stu.Unpack (TestIn.GetBuffer());
	stu.Print(cout, "record");
    }	
    
    for(int i=0; i<10; i++){
	TestIn.Read(cr_recaddr[i]);
	cou.Unpack (TestIn.GetBuffer());
	cou.Print(cout, "record");
    }
	
    TestIn.Close();

}

void testFixedField ()
{
	cout <<"Testing Fixed Field Buffer"<<endl;
	FixedFieldBuffer Buff (6);
	student :: InitBuffer (Buff);
    coursereg :: InitBuffer (Buff);
	testBufferFile(Buff, "fixlen2.dat");
}


void testLength ()
{
	cout << "\nTesting LengthTextBuffer"<<endl;
	LengthFieldBuffer Buff;
    student :: InitBuffer (Buff);
    coursereg :: InitBuffer (Buff);
	testBufferFile(Buff, "length2.dat");
}


void testDelim ()
{
	cout << "\nTesting DelimTextBuffer"<<endl;
	DelimFieldBuffer::SetDefaultDelim ('|');
	DelimFieldBuffer Buff;
	
    student :: InitBuffer (Buff);
    coursereg :: InitBuffer (Buff);
    testBufferFile(Buff, "delim2.dat");
}

int main(int argc, char ** argv)
{
	InitPerson();
	testFixedField ();
	testLength ();
	testDelim ();
}
