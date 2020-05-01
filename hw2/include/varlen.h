#ifndef VARLEN_H
#define VARLEN_H

#include <cstdlib>
#include <iostream>
#include "iobuffer.h"

using namespace std;


// Abstract class designed to support variablelength records
// Fields may be of a variety of types
class VariableLengthBuffer: public IOBuffer
{  public:
	VariableLengthBuffer (int MaxBytes = 1000);
 	int Init (); 
	void Clear (); // clear fields from buffer

	int Read (istream &);
	int Write (ostream &) const;
	int SizeOfBuffer () const; // return current size of buffer
	void Print (ostream &) const;

};

#endif
