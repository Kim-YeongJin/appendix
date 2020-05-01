// fixfld.cpp

#include "fixfld.h"
#include <cstring>

//class FixedFieldBuffer

// public members

FixedFieldBuffer :: FixedFieldBuffer (int maxFields, int maxBytes)
 // construct with a maximum of maxFields
: FixedLengthBuffer(maxBytes)
{
	Init (maxFields);
}

// private function to calculate the record size from the field sizes
static int SumFieldSizes (int numFields, int * fieldSize)
{
	int sum = 0;
	for (int i = 0; i < numFields; i++)
		sum += fieldSize[i];
	return sum;
}

// construct with fields of specific size
FixedFieldBuffer :: FixedFieldBuffer (int numFields, int * fieldSize) 
: FixedLengthBuffer(SumFieldSizes(numFields, fieldSize))
{
	Init (numFields, fieldSize);
}


// clear fields from buffer
void FixedFieldBuffer :: Clear ()
{
	FixedLengthBuffer::Clear ();
	NextField = 0;
	Buffer[0]=0;
	Packing = TRUE;
}

 // construct with a maximum of maxFields
int FixedFieldBuffer :: Init (int maxFields)
{
	Clear ();
	if (maxFields < 0) maxFields = 0;
	MaxFields = maxFields;
	FieldSize = new int[MaxFields];
	BufferSize = 0;
	NumFields = 0;
	return 1;
}

// construct with fields of specific size
int FixedFieldBuffer :: Init (int numFields, int * fieldSize) 
{
	// initialize
	Initialized = TRUE;
	Init (numFields);

	// add fields
	for (int j = 0; j < numFields; j++)
		AddField (FieldSize[j]);
	return TRUE;
}



int FixedFieldBuffer::AddField (int fieldSize)
{
	Initialized = TRUE;
	if (NumFields == MaxFields) return FALSE;
	if (BufferSize + fieldSize > MaxBytes) return FALSE;
	FieldSize[NumFields] = fieldSize;
	NumFields ++;
	BufferSize += fieldSize;
	return TRUE;
}

static const char * headerStr = "Field";
static const int headerStrSize = strlen (headerStr);

// read the header and check for consistency
// see WriteHeader for header record structure
int FixedFieldBuffer :: ReadHeader (istream & stream)
{
	char * str = new char[headerStrSize+1];
	int numFields, *fieldSize;
	int result;
	// read the FixedLengthBufferheader
	result = FixedLengthBuffer::ReadHeader(stream);
	if (result < 0) return -1;
	// read the header string 
	stream.read (str, headerStrSize);
	if (!stream.good()) return -1;
	if (strncmp (str, headerStr, headerStrSize) != 0) return -1;
	// read the record description
	stream.read ((char*)&numFields, sizeof(numFields));
	if (!stream) return -1; // failed to read numFields
	fieldSize = new int[numFields];
	for (int i = 0; i < numFields; i ++)
	{
		stream.read ((char*)&fieldSize[i], sizeof(fieldSize[i]));
	}

	if (Initialized) // check header for consistency
	{
		if (numFields != NumFields) return -1;
		for (int j = 0; j < numFields; j ++)
			if (fieldSize[j] != FieldSize[j]) return -1;
		return stream.tellg(); // everything matches
	}
	// else initialize the buffer from the header
	Init (numFields, fieldSize);
	return stream.tellg();
}

// write a buffer header to the beginning of the stream
// A header consists of the 
//	FixedLengthBufferheader	
//	FIXED			5 bytes
//	Variable sized record of length fields
//	that describes the file records
//	Header record size	2 bytes
//	number of fields		4 bytes
//	field sizes			4 bytes per field
int FixedFieldBuffer :: WriteHeader (ostream & stream) const
{
	int result;
	if (!Initialized) return -1; // cannot write unitialized buffer
	// write the parent (FixedLengthBuffer) header
	result = FixedLengthBuffer::WriteHeader (stream);
	if (!result) return -1;
	// write the header string 
	stream.write (headerStr, headerStrSize);
	if (!stream.good()) return -1;
	// write the record description
	//cout << "packing numfields "<<NumFields<<endl;
	stream.write ((char*)&NumFields, sizeof(NumFields));
	for (int i = 0; i < NumFields; i ++)
	{
		//cout << "packing fieldsize "<<FieldSize[i]<<endl;
		stream.write ((char*)&FieldSize[i], sizeof(FieldSize[i]));
	}
	if (!stream) return -1;
	return stream.tellp ();
}





// set the value of the next field of the buffer;
//    if size != -1, it must be the same as the packSize
// return number of bytes packed, -1 if error
int FixedFieldBuffer :: Pack (const void * field, int size)
{
//cout<<"Pack NumFields "<<NumFields<<" field "<<(char *)field<<endl;
	if (NextField == NumFields || !Packing) // buffer is full or not packing mode
		return -1;
	int start = NextByte; // first byte to be packed
	int packSize = FieldSize[NextField]; // number bytes to be packed
	if (size != -1 && packSize != size) return -1; 
	memcpy (&Buffer[start], field, packSize); // move bytes to buffer
	NextByte += packSize;
	NextField ++;
	if (NextField == NumFields) // all fields packed
	{
		Packing = -1;
		NextField = NextByte = 0;
	}
	return packSize;
}

// extract the value of the next field of the buffer
// return the number of bytes extracted, -1 if error
int FixedFieldBuffer :: Unpack (void * field, int maxBytes) 
{
	Packing = FALSE;
	if (NextField == NumFields) // buffer is full 
		return -1;
	int start = NextByte; // first byte to be unpacked
	int packSize = FieldSize[NextField]; // number bytes to be unpacked
	memcpy (field, &Buffer[start], packSize);
	NextByte += packSize;
	NextField ++;
	if (NextField == NumFields) Clear (); // all fields unpacked
	return packSize;
}

void FixedFieldBuffer :: Print (ostream & stream) const
{
	FixedLengthBuffer::Print (stream);
	stream << endl;
	stream << "\t max fields "<<MaxFields<<" and actual "<<NumFields<<endl;
	for (int i = 0; i < NumFields; i++)
		stream <<"\tfield "<<i<<" size "<<FieldSize[i]<<endl;
	Buffer[BufferSize]=0;
	stream <<"NextByte "<<NextByte<<endl;
	stream <<"Buffer '"<<Buffer<<"'"<<endl;
}

