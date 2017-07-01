// fixtext.cc
// Copyright 1997, Gregory A. Riccardi

using namespace std;
#include "fixtext.h"
#include <string.h>

//class FixedTextBuffer 

// public members

FixedTextBuffer::FixedTextBuffer(int maxFields, int maxChars)
// construct with a maximum of maxFields
		{
	Init(maxFields, maxChars);
}

FixedTextBuffer::FixedTextBuffer(int numFields, int * fieldSize)
// construct with fields of specific size
		{
	Init(numFields, fieldSize);
}

int FixedTextBuffer::NumberOfFields() const
// return number of fields 
{
	return NumFields;
}

void FixedTextBuffer::Clear()
// clear fields from buffer
{
	NextField = 0;
	NextCharacter = 0;
	Packing = TRUE;
	buffer[0] = 0;
}

int FixedTextBuffer::AddField(int fieldSize){
	if(NumFields == MaxFields)
		return FALSE;
	if(bufferSize + fieldSize > MaxChars)
		return FALSE;
	FieldSize[NumFields] = fieldSize;
	NumFields++;
	bufferSize += fieldSize;
	return TRUE;
}

int FixedTextBuffer::Read(istream & stream){
	stream.read(buffer, bufferSize);
	return stream.good();
}

int FixedTextBuffer::Write(ostream & stream) const{
	stream.write(buffer, bufferSize);
	return stream.good();
}

int FixedTextBuffer::Pack(const char * str, int size)
// set the value of the next field of the buffer;
		{
	if(NextField == NumFields || !Packing) // buffer is full or not packing mode
		return FALSE;
	int len = strlen(str);
	int start = NextCharacter; // first byte to be packed
	int packSize = FieldSize[NextField]; // number bytes to be packed
	strncpy(&buffer[start], str, packSize);
	NextCharacter += packSize;
	NextField++;
	// if len < packSize, pad with blanks
	for(int i = start + packSize; i < NextCharacter; i++)
		buffer[start] = ' ';
	buffer[NextCharacter] = 0; // make buffer look like a string
	if(NextField == NumFields) // buffer is full
	{
		Packing = FALSE;
		NextField = NextCharacter = 0;
	}
	return TRUE;
}

int FixedTextBuffer::Unpack(char * str)
// extract the value of the next field of the buffer
		{
	if(NextField == NumFields || Packing) // buffer is full or not unpacking mode
		return FALSE;
	int start = NextCharacter; // first byte to be unpacked
	int packSize = FieldSize[NextField]; // number bytes to be unpacked
	strncpy(str, &buffer[start], packSize);
	str[packSize] = 0; // terminate string with zero
	NextCharacter += packSize;
	NextField++;
	if(NextField == NumFields)
		Clear(); // all fields unpacked
	return TRUE;
}

void FixedTextBuffer::Print(ostream & stream) const{
	stream << "buffer has max fields " << MaxFields << " and actual " << NumFields << endl << "max bytes " << MaxChars << " and buffer Size " << bufferSize << endl;
	for(int i = 0; i < NumFields; i++)
		stream << "\tfield " << i << " size " << FieldSize[i] << endl;
	if(Packing)
		stream << "\tPacking\n";
	else
		stream << "\tnot Packing\n";
	stream << "Contents: '" << buffer << "'" << endl;
}

// Protected members

int FixedTextBuffer::Init(int maxFields, int maxChars)
// construct with a maximum of maxFields
		{
	if(maxFields < 0)
		maxFields = 0;
	if(maxChars < 0)
		maxChars = 0;
	MaxFields = maxFields;
	MaxChars = maxChars;
	FieldSize = new int[MaxFields];
	buffer = new char[MaxChars];
	bufferSize = 0;
	NumFields = 0;
	NextField = 0;
	Packing = TRUE;
	return 1;
}

int FixedTextBuffer::Init(int numFields, int * fieldSize)
// construct with fields of specific size
		{
	// calculate buffer size
	int bufferSize = 1;
	for(int i = 0; i < numFields; i++)
		bufferSize += fieldSize[i];

	// initialize
	Init(numFields, bufferSize);

	// add fields
	for(int j = 0; j < numFields; j++)
		AddField (FieldSize[j]);
	return 1;
}

