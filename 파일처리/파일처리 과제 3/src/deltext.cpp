// deltext.cpp

// Copyright 1997, Gregory A. Riccardi
using namespace std;
#include "deltext.h"
#include <string.h>

DelimTextBuffer::DelimTextBuffer(char delim, int maxBytes)
		: IOBuffer(maxBytes){
	Init(delim, maxBytes);
}

void DelimTextBuffer::Clear()
// clear fields from buffer
{
	NextByte = 0;
	bufferSize = 0;
}

int DelimTextBuffer::Read(istream & stream){
	Clear();
	stream.read((char*) &bufferSize, sizeof(bufferSize));
	if(stream.fail())
		return false;
	if(bufferSize > maxBytes)
		return false; // buffer overflow
	stream.read(buffer, bufferSize);
	return stream.good();
}

int DelimTextBuffer::Write(ostream & stream) const{
	stream.write((char*) &bufferSize, sizeof(bufferSize));
	stream.write(buffer, bufferSize);
	return stream.good();
}

int DelimTextBuffer::Pack(const char * str, int size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(str) as Delim of field
		{
	short len; // length of string to be packed
	if(size >= 0)
		len = size;
	else
		len = strlen(str);
	if(len > strlen(str)) // str is too short!
		return false;
	int start = NextByte; // first character to be packed
	NextByte += len + 1;
	if(NextByte > maxBytes)
		return false;
	memcpy(&buffer[start], str, len);
	buffer[start + len] = Delim; // add delimeter
	bufferSize = NextByte;
	return true;
}

int DelimTextBuffer::Unpack(char * str)
// extract the value of the next field of the buffer
		{
	int len = -1; // length of packed string
	int start = NextByte; // first character to be unpacked
	for(int i = start; i < bufferSize; i++)
		if(buffer[i] == Delim){
			len = i - start;
			break;
		}
	if(len == -1)
		return false; // delimeter not found
	NextByte += len + 1;
	if(NextByte > bufferSize)
		return false;
	strncpy(str, &buffer[start], len);
	str[len] = 0; // zero termination for string 
	return true;
}

void DelimTextBuffer::Print(ostream & stream) const{
	stream << "buffer has max characters " << maxBytes << " and buffer Size " << bufferSize << endl;
}

// Protected members

int DelimTextBuffer::Init(char delim, int maxBytes)
// construct with a maximum of maxFields
		{
	Delim = delim;
	DelimStr[0] = Delim;
	DelimStr[1] = 0;
	return 1;
}

