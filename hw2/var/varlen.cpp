#include "varlen.h"
#include <cstring>

//class VariableLengthBuffer

// public members

// construct with a maximum of maxFields
VariableLengthBuffer::VariableLengthBuffer (int maxBytes)
: IOBuffer (maxBytes)
{
	Init();
}

// clear fields from buffer
void VariableLengthBuffer::Clear ()
{
	IOBuffer::Clear();
}

// write the number of bytes in the buffer field definitions
// the record length is represented by an unsigned short value
int VariableLengthBuffer::Read (istream & stream)
{
	if (stream.eof()) return -1;
	int recaddr = stream.tellg ();
	Clear();
	unsigned short bufferSize;
	stream.read ((char *)&bufferSize, sizeof(bufferSize));
	if ( !stream.good() ) {stream.clear(); return -1;}
	BufferSize = bufferSize;
	if (BufferSize > MaxBytes) return -1; // buffer overflow
	stream.read (Buffer, BufferSize);
	if ( !stream.good() ) {stream.clear(); return -1;}
	return recaddr;
}


// write the length and buffer into the stream
int VariableLengthBuffer::Write(ostream & stream) const
{
	int recaddr = stream.tellp();
	unsigned short bufferSize = BufferSize;
	stream.write((char *)&bufferSize, sizeof(bufferSize));
	if ( !stream ) return -1;
	stream.write(Buffer, BufferSize);
	if ( !stream.good() ) return -1;
	return recaddr;
}



void VariableLengthBuffer::Print (ostream & stream) const
{
	IOBuffer::Print(stream);
}

// construct with a maximum of maxFields
int VariableLengthBuffer::Init()
{
	Clear();
	return TRUE;
}

