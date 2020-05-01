// fixlen.cpp

#include "fixlen.h"
#include <cstring>

//class FixedLengthBuffer

// public members

 // construct with a maximum of maxFields
FixedLengthBuffer::FixedLengthBuffer (int recordSize)
: IOBuffer (recordSize)
{
	Init (recordSize);
}

// clear fields from buffer
void FixedLengthBuffer::Clear ()
{
	IOBuffer::Clear ();
	Buffer[0]=0;
	Packing = TRUE;
}

// write the number of bytes in the buffer field definitions
int FixedLengthBuffer::Read(istream & stream)
{
	int recaddr = stream.tellg (); stream.clear();
	Clear ();
	Packing = FALSE;
	stream.read(Buffer, BufferSize);
	if ( !stream.good() ) {stream.clear(); return recaddr;}
	return recaddr;
}


// read the number of bytes in the buffer field definitions
// return the location of the record in the file
int FixedLengthBuffer::Write(ostream & stream) const
{
	int recaddr = stream.tellp ();
	stream.write (Buffer, BufferSize);
	if (! stream.good()) return -1;
	return recaddr;
}

static const char * headerStr = "Fixed";
static const int headerStrSize = strlen (headerStr);

// read the header and check for consistency
// see WriteHeader for header record structure
int FixedLengthBuffer::ReadHeader (istream & stream)
{
	char str[headerStrSize+1];
	int recordSize;
	int result;
	// read the IOBuffer header
	result = IOBuffer::ReadHeader (stream);
	if (result < 0) return -1;
	// read the string "Fixed"
	stream.read(str, headerStrSize);
	if (!stream.good()) return -1;
	if (strncmp(str, headerStr, headerStrSize) != 0) return -1;
	stream.read((char*)&recordSize, sizeof(recordSize));
	if (Initialized) // check header for consistency
	{
		if (recordSize != BufferSize) return -1;
	}
	// else initialize the buffer from the header
	ChangeRecordSize(recordSize);
	return stream.tellg();
}

// write a buffer header to the beginning of the stream
// A header consists of the 
//	IOBUFFER header	
//	FIXED			5 bytes
//	record size		2 bytes
int FixedLengthBuffer::WriteHeader (ostream & stream) const
{
	int result;
	if (!Initialized) return -1; // cannot write unitialized buffer
	// write the parent (IOBuffer) header
	result = IOBuffer::WriteHeader(stream);
	if (!result) return -1;
	// write the string "Fixed"
	stream.write (headerStr, headerStrSize);
	if (!stream.good()) return -1;
	// write the record size
	stream.write((char *)&BufferSize, sizeof(BufferSize));
	if (!stream.good ()) return -1;
	return stream.tellp ();
}


void FixedLengthBuffer::Print(ostream & stream) const
{
	IOBuffer::Print (stream);
	stream <<  "Fixed ";
}

// construct with a maximum of maxFields
int FixedLengthBuffer::Init(int recordSize)
{
	Clear ();
	BufferSize = recordSize;
	return 1;
}

// construct with a maximum of maxFields
int FixedLengthBuffer::ChangeRecordSize (int recordSize)
{
	BufferSize = recordSize;
	return 1;
}
