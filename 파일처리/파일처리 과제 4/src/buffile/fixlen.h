#ifndef FIXLEN_H
#define FIXLEN_H

using namespace std;
#include <stdlib.h>
#include <iostream>
#include "fixtext.h"

class FixedLengthBuffer : public IOBuffer
// Abstract class designed to support fixed length records
{
	public:
		FixedLengthBuffer(int recordSize = 1000);
		FixedLengthBuffer(const FixedLengthBuffer & buffer); // copy constructor

		void Clear(); // clear values from buffer
		int Pack(const char * field, int size = -1);
		int Unpack(char * field, int maxbytes = -1);
		int Read(istream &);
		int Write(ostream &) const;
		int ReadHeader(istream &); // read header from stream
		int WriteHeader(ostream &) const; // write a header to the stream
		void Print(ostream &) const;
		int SizeOfBuffer() const; // return size of buffer
		FixedTextBuffer& getBuffer();
		int getBufferSize();

	protected:
		int Init(int recordSize);
		int ChangeRecordSize(int recordSize);

		FixedTextBuffer fixbuf;
};

inline FixedLengthBuffer::FixedLengthBuffer(const FixedLengthBuffer & buffer)
		: IOBuffer(buffer){
	Init(buffer.BufferSize);
}

#endif
