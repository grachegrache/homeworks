#include "IOBuffer.h"

IOBuffer::~IOBuffer(){
	delete[] buffer;
	maxBytes = 0;
	buffer = 0;
	bufferSize = 0;
}
