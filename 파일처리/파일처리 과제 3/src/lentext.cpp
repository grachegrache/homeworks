// lentext.cc
// Copyright 1997, Gregory A. Riccardi

using namespace std;
#include "lentext.h"
#include <string.h>

LengthTextBuffer::LengthTextBuffer(int maxBytes)
// construct with a maximum of maxFields
		: IOBuffer(maxBytes){
	Init();
}

void LengthTextBuffer::Clear()
// clear fields from buffer
{
	NextByte = 0;
	bufferSize = 0;
}

int LengthTextBuffer::Read(istream & stream){
	Clear();
	stream.read((char*) &bufferSize, sizeof(bufferSize));
	if(stream.fail())
		return FALSE;
	if(bufferSize > maxBytes)
		return FALSE; // buffer overflow
	stream.read(buffer, bufferSize);
	return stream.good();
}

int LengthTextBuffer::Write(ostream & stream) const{
	stream.write((char*) &bufferSize, sizeof(bufferSize));
	stream.write(buffer, bufferSize);
	return stream.good();
}

int LengthTextBuffer::Pack(const char * str, int size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(str) as length of field
		{
	short len; // length of string to be packed
	if(size >= 0)
		len = size;
	else
		len = strlen(str);

	if(len > strlen(str)) // str is too short!
		return FALSE;

	int start = NextByte; // first character to be packed
	NextByte += (len + sizeof(len));
	if(NextByte > maxBytes)
		return FALSE;
	memcpy(&buffer[start], &len, sizeof(len));
	strncpy(&buffer[start + sizeof(len)], str, len);
	bufferSize = NextByte;
	return TRUE;
}

int LengthTextBuffer::Unpack(char * str)
// extract the value of the next field of the buffer
		{
	short len; // length of packed string
	if(NextByte >= bufferSize)
		return FALSE; // no more fields
	int start = NextByte; // first character to be unpacked
	memcpy(&len, &buffer[start], sizeof(short));
	NextByte += len + sizeof(short);
	if(NextByte > bufferSize)
		return FALSE;
	strncpy(str, &buffer[start + sizeof(short)], len);
	str[len] = 0; // zero termination for string 
	return TRUE;
}

void LengthTextBuffer::Print(ostream & stream) const{
	stream << "buffer has characters " << maxBytes << " and buffer Size " << bufferSize << endl;
}

int LengthTextBuffer::Init()
// construct with a maximum of maxFields
{
	Clear();
	return 1;
}
