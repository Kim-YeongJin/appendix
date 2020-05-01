#include "student.h"

student::student() {Clear ();}

void student::Clear ()
{
	// set each field to an empty string
	identifier [0] = 0; name [0] = 0; Address [0] = 0;
	dofe [0] = 0; credit_hours [0] = 0;
}

int student::Pack (IOBuffer & Buffer) const
{// pack the fields into a FixedFieldBuffer, 
 // return TRUE if all succeed, FALSE o/w
	int numBytes;
	Buffer . Clear ();
	numBytes = Buffer . Pack (identifier);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer . Pack (name);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer . Pack (Address);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer . Pack (dofe);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer . Pack (credit_hours);
	if (numBytes == -1) return FALSE;
	return TRUE;
}

int student::Unpack (IOBuffer & Buffer)
{
	Clear ();
	int numBytes;
	numBytes = Buffer . Unpack (identifier);
	if (numBytes == -1) return FALSE;
	identifier[numBytes] = 0;
	numBytes = Buffer . Unpack (name);
	if (numBytes == -1) return FALSE;
	name[numBytes] = 0;
	numBytes = Buffer . Unpack (Address);
	if (numBytes == -1) return FALSE;
	Address[numBytes] = 0;
	numBytes = Buffer . Unpack (dofe);
	if (numBytes == -1) return FALSE;
	dofe[numBytes] = 0;
	numBytes = Buffer . Unpack (credit_hours);
	if (numBytes == -1) return FALSE;
    credit_hours[numBytes] = 0;
	return TRUE;
}

int student::InitBuffer(FixedFieldBuffer & Buffer)
// initialize a FixedFieldBuffer to be used for Persons
{
	int result;
	result = Buffer.AddField (10); // LastName [11];
	result = result && Buffer.AddField (10); // FirstName [11];
	result = result && Buffer.AddField (15); // Address [16];
	result = result && Buffer.AddField (15); // City [16];
	result = result && Buffer.AddField (2);  // State [3];
	result = result && Buffer.AddField (9); // ZipCode [10];
	return result;
}

int student::InitBuffer (DelimFieldBuffer & Buffer)
// initialize a DelimFieldBuffer to be used for Persons
{	return TRUE;}

// initialize a LengthFieldBuffer to be used for Persons
int student::InitBuffer (LengthFieldBuffer & Buffer)
{	return TRUE;}


void student::Print (ostream & stream, char * label) const
{
	if (label == 0) stream << "Student:";
	else stream << label;
	stream << "\n\t id '"<<identifier<<"'\n"
		<< "\tname '"<<name<<"'\n"
		<< "\t   Address '"<<Address<<"'\n"
		<< "\t      dofe '"<<dofe<<"'\n"
		<< "\t     credit_hours '"<<credit_hours<<"'\n"<<flush;
}

