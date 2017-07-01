// fixtext.h
// Copyright 1997, Gregory A. Riccardi

#ifndef FIXTEXT_H
#define FIXTEXT_H

#include "iobuffer.h"

class FixedTextBuffer : public IOBuffer
// a buffer which holds a specific number of fixed sized text fields.
{
	public:
		FixedTextBuffer(int maxFields = 10, int maxChars = 1000); // construct with a maximum of maxFields
		FixedTextBuffer(int numFields, int * FieldSize);
		// construct with fields of specific size
		int NumberOfFields() const; // return number of fields 
		void Clear(); // clear field values from buffer
		int AddField(int fieldSize);
		int Read(istream &);
		int Write(ostream &) const;
		int Pack(const char *, int size = -1); // set the value of the next field of the buffer;
		int Unpack(char *, int maxBytes = -1); // extract the value of the next field of the buffer
		void Print(ostream &) const;
		int Init(int numFields, int maxChars = 1000);
		int Init(int numFields, int * fieldSize);

		char* getBuffer();
		int isEqualField(int fieldNum, const char* field);
		void setPacking(int _bool);
	private:
//		char * Buffer; // character array to hold field values
//		int BufferSize; // sum of the sizes of declared fields
		int * FieldSize; // array to hold field sizes
		int MaxFields; // maximum number of fields
		int MaxChars; // maximum number of characters in the buffer
		int NumFields; // actual number of defined fields
		int NextField; // index of next field to be packed/unpacked
		int NumFieldValues; // number of fields which are packed
		int Packing; // TRUE if in packing phase, FALSE o/w
		int NextCharacter; // packing/unpacking position in buffer
};

#endif
