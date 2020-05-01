#ifndef COURSEREG_H
#define COURSEREG_H

#include <iostream>
#include "fixfld.h"
#include "delim.h"
#include "length.h"

class coursereg
{
  public:
	// fields
	char course_id [11];
	char student_id [11];
	char course_credit_hours [16];
	char course_grade [16];
	
	//operations
    coursereg();
	void Clear ();

	static int InitBuffer (DelimFieldBuffer &);
	static int InitBuffer (LengthFieldBuffer &);
	static int InitBuffer (FixedFieldBuffer &);

	int Unpack (IOBuffer &);
	int Pack (IOBuffer &) const;
	void Print (ostream &, char * label = 0) const;
};

#endif
