#include "delim.h"
#include <cstring>

// class DelimFieldBuffer

// public members

// construct with a maximum of maxFields
DelimFieldBuffer :: DelimFieldBuffer(char delim, int maxBytes)
: VariableLengthBuffer (maxBytes)
{
	Init(delim);
}

// clear fields from buffer
void DelimFieldBuffer :: Clear ()
{
	VariableLengthBuffer::Clear();
}

// construct with a maximum of maxFields
int DelimFieldBuffer :: Init (char delim)
{
	Initialized = TRUE;
	Clear ();
	if (delim == -1) Delim = DefaultDelim;
	else Delim = delim;
	return TRUE;
}

// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(field) as Delim of field
// return number of bytes packed, -1 if error
int DelimFieldBuffer :: Pack (const void * field, int size)
{
	// look for delimeter in field!
	short len; // length of string to be packed
	if (size >= 0) len = size;
	else len = strlen ((char*)field);
	if (len > (short)strlen((char*)field)) // field is too short!
		return -1;
	int start = NextByte; // first character to be packed
	NextByte += len + 1;
	if (NextByte > MaxBytes) return -1;
	memcpy (&Buffer[start], field, len);
	Buffer [start+len] = Delim; // add delimeter
	BufferSize = NextByte;
	return len;
}

// extract the value of the next field of the buffer
// return the number of bytes extracted, -1 if error
int DelimFieldBuffer :: Unpack (void * field, int maxBytes)
{
	int len = -1; // length of packed string
	int start = NextByte; // first character to be unpacked
	for (int i = start; i < BufferSize; i++)
		if (Buffer[i] == Delim) 
			{len = i - start; break;}
	if (len == -1) return -1; // delimeter not found
	NextByte += len + 1;
	if (NextByte > BufferSize) return -1;
	// check for maxBytes
	memcpy (field, &Buffer[start], len);
	if (maxBytes > len || maxBytes == -1)
		((char*)field) [len] = 0; // zero termination for string 
	return len;
}

void DelimFieldBuffer :: Print (ostream & stream) const
{
	VariableLengthBuffer::Print (stream);
	stream << " Delimeter '"<<Delim<<"'"<<endl;
}


void DelimFieldBuffer :: SetDefaultDelim (char delim)
{
	DefaultDelim = delim;
}

// initialize static protected element

char DelimFieldBuffer :: DefaultDelim = 0;