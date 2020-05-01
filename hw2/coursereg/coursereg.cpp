#include "coursereg.h"

coursereg::coursereg() {Clear ();}

void coursereg::Clear ()
{
	// set each field to an empty string
	course_id [0] = 0; student_id [0] = 0; course_credit_hours [0] = 0;
	course_grade [0] = 0;
}

int coursereg::Pack (IOBuffer & Buffer) const
{// pack the fields into a FixedFieldBuffer, 
 // return TRUE if all succeed, FALSE o/w
	int numBytes;
	Buffer . Clear ();
	numBytes = Buffer . Pack (course_id);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer . Pack (student_id);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer . Pack (course_credit_hours);
	if (numBytes == -1) return FALSE;
	numBytes = Buffer . Pack (course_grade);
	if (numBytes == -1) return FALSE;
	return TRUE;
}

int coursereg::Unpack (IOBuffer & Buffer)
{
	Clear ();
	int numBytes;
	numBytes = Buffer . Unpack (course_id);
	if (numBytes == -1) return FALSE;
	course_id[numBytes] = 0;
	numBytes = Buffer . Unpack (student_id);
	if (numBytes == -1) return FALSE;
	student_id[numBytes] = 0;
	numBytes = Buffer . Unpack (course_credit_hours);
	if (numBytes == -1) return FALSE;
	course_credit_hours[numBytes] = 0;
	numBytes = Buffer . Unpack (course_grade);
	if (numBytes == -1) return FALSE;
	course_grade[numBytes] = 0;
	return TRUE;
}

int coursereg::InitBuffer(FixedFieldBuffer & Buffer)
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

int coursereg::InitBuffer (DelimFieldBuffer & Buffer)
// initialize a DelimFieldBuffer to be used for Persons
{	return TRUE;}

// initialize a LengthFieldBuffer to be used for Persons
int coursereg::InitBuffer (LengthFieldBuffer & Buffer)
{	return TRUE;}


void coursereg::Print (ostream & stream, char * label) const
{
	if (label == 0) stream << "Course Reg:";
	else stream << label;
	stream << "\n\t course_id '"<<course_id<<"'\n"
		<< "\tstudent_id '"<<student_id<<"'\n"
		<< "\t   course_credit_hours '"<<course_credit_hours<<"'\n"
		<< "\t      course_grade '"<<course_grade<<"'\n"<<flush;
}

