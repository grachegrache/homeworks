#ifndef _IOBUFFER_H_
#define _IOBUFFER_H_

using namespace std;
#include "iostream"

#ifndef FALSE
#define FALSE (0)
#define TRUE (1)
#endif

class IOBuffer{
	public:
		IOBuffer(int maxBytes = 1000){
			if(maxBytes < 0)
				maxBytes = 0;
			this->maxBytes = maxBytes;
			buffer = new char[maxBytes];
			bufferSize = 0;
		}
		virtual ~IOBuffer();
		virtual int Pack(const char* field, int size = -1) = 0;
		virtual int Unpack(char* field) = 0;
		virtual int Read(istream&) = 0;
		virtual int Write(ostream&) const = 0;
		virtual void Clear() = 0;
		virtual void Print(ostream & stream) const = 0;

	protected:
		char* buffer;
		int bufferSize;
		int maxBytes;
};

#endif
