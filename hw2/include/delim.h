#ifndef DELIM_H
#define DELIM_H

#include <iostream>
#include "varlen.h"

using namespace std;

// a buffer which holds delimited text fields.
// Record variables can be packed into and extracted from a buffer.
// Input and Output of packed buffers 
// 
// To use this class, create a DelimFieldBuffer variable and associate definitions with the fields.
// operations are provided to allow values to be associated with the fields (Pack)
//	and to fetch the values of fields (Unpack)
class DelimFieldBuffer: public VariableLengthBuffer
{  
public:
	DelimFieldBuffer (char Delim = -1, int maxBytes = 1000); // construct with a maximum of maxBytes
		// construct with fields with delimeters
	DelimFieldBuffer (const DelimFieldBuffer & buffer); // copy constructor

	void Clear (); // clear fields from buffer
	int Init (char delim = 0);

	int Pack (const void*, int size = -1); // set the value of the next field of the buffer;
	int Unpack (void * field, int maxBytes = -1); // extract the value of the next field of the buffer

	void Print (ostream &) const;
	static void SetDefaultDelim (char delim);
 protected:
	char Delim;
	static char DefaultDelim;
};

#endif
