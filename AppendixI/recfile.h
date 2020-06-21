#ifndef RECFILE_H
#define RECFILE_H

#include "buffile.h"
#include "iobuffer.h"
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (IOBuffer &); pack record into buffer
//	int Unpack (IOBuffer &); unpack record from buffer

template <class RecType>
class RecordFile: public BufferFile
{public:
	int Read(RecType & record, int recaddr = -1);
	int Write(const RecType & record, int recaddr = -1);
	int Append(const RecType & record);
	RecordFile(IOBuffer & buffer): BufferFile (buffer) {}
};

// template method bodies
template <class RecType>
int RecordFile<RecType>::Read(RecType & record, int recaddr)
{
	int writeAddr, result;
	writeAddr = BufferFile::Read (recaddr);
	if (!writeAddr) return -1;
	result = record . Unpack (Buffer);
	if (!result) return -1;
	return writeAddr;
}


template <class RecType>
int RecordFile<RecType>::Write (const RecType & record, int recaddr)
{
	int result;
	result = record . Pack (Buffer);
	if (!result) return -1;
	return BufferFile::Write (recaddr);
}

template <class RecType>
int RecordFile<RecType>::Append (const RecType & record)
{
	int result;
	result = record . Pack (Buffer);
	if (!result) return -1;
	return BufferFile::Append();
}

#endif