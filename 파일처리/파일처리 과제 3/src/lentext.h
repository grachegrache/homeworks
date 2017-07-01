// lentext.h
// Copyright 1997, Gregory A. Riccardi

#ifndef LENTEXT_H
#define LENTEXT_H

#include "IOBuffer.h"

class LengthTextBuffer : public IOBuffer
// a buffer which holds length-based text fields.
{
	public:
		LengthTextBuffer(int maxBytes = 1000);
		// construct with a maximum of maxBytes
		void Clear(); // clear fields from buffer
		int Read(istream &);
		int Write(ostream &) const;
		int Pack(const char *, int size = -1);
		// set the value of the next field of the buffer;
		int Unpack(char *);
		// extract the value of the next field of the buffer
		void Print(ostream &) const;
		int Init();
	private:
//		char * Buffer; // character array to hold field values
//		int BufferSize; // size of packed fields
//		int MaxBytes; // maximum number of characters in the buffer
		int NextByte; // packing/unpacking position in buffer
};

#endif
