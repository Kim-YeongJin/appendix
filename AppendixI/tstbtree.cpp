//tstbtree.cc

#include "btnode.h"
#include "btree.h"
#include <iostream.h>

// ¹®ÀÚ¿­ Key
const char key[4][10];
cout<<"Enter the keys"<<endl;
for(int i=0; i<4; i++)
cin>>key[i];
const int BTreeSize = 4;

int main (int argc, char * argv)
{
	int result_1,result_2, i;
	BTree<char> bt((int)BTreeSize);
	result_1 = bt.Create ("testbt.dat",ios::in|ios::out);
	if (!result){cout<<"Please delete testbt.dat"<<endl;return 0;}
	for (i = 0; i<26; i++)
	{
		cout<<"Inserting "<<keys[i]<<endl;
		result = bt.Insert(keys[i],i);
		bt.Print(cout);
	}
	bt.Search(1, 1);


    // Test module of deleting key.
	BTree<char> ct((int)BTreeSize);
	result_2 = ct.Create ("testct.dat",ios::in|ios::out);
	if (!result)
	{
		cout<<"Please delete testct.dat"<<endl;
		return 0;
	}
	for (i = 0; i<26; i++)
	{
		cout<<"Deleting "<<keys[i]<<endl;
		result_2 = ct.Insert(keys[i],i);
		ct.Print(cout);
	}
	ct.Search(1, 1);


	return 1;
}


