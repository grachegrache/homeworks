using namespace std;
#include "buffile.h"
#include "fixlen.h"
#include <string.h>

BufferFile::BufferFile(FixedLengthBuffer& from)
		// create with a buffer
		: Buffer(from){
	prevDeleted = 0;
}

int BufferFile::Open(char * filename, int mode)
// open an existing file and check the header
// a correct header must be on the file
// use ios::nocreate to ensure that a file exists
		{
	// these modes are not allowed when opening an existing file
	if(mode & ios::trunc)
		return FALSE;

	File.open(filename, (_Ios_Openmode)(mode | ios::in | ios::out));
	if(!File.good())
		return FALSE;

	File.seekg(0, ios::beg);
	File.seekp(0, ios::beg);
	HeaderSize = ReadHeader();
	if(!HeaderSize) // no header and file opened for output
		return FALSE;

	File.seekp(HeaderSize, ios::beg);
	File.seekg(HeaderSize, ios::beg);

	return File.good();
}

int BufferFile::Create(char * filename, int mode)
// create a new file and write a header on it.
// use ios::nocreate to ensure that no file exists
		{
	if(!(mode & ios::out))
		return FALSE; // must include ios::out

	File.open(filename, (_Ios_Openmode)(mode | ios::out));
	if(!File.good()){
		File.close();
		return FALSE;
	}

	HeaderSize = WriteHeader();
	return HeaderSize != 0;
}

int BufferFile::Close(){
	File.close();
	return TRUE;
}

int BufferFile::Rewind(){
	File.seekg(HeaderSize, ios::beg);
	File.seekp(HeaderSize, ios::beg);
	return 1;
}

// Input and Output operations
int BufferFile::Read(int recaddr)
// read a record into the buffer
// return the record address
// return <0 if read failed
// if recaddr == -1, read the next record in the File
// if recaddr != -1, read the record at that address
		{
	if(!File.fail()){
		char field[5];
		File.read(field, 5);

		if(strstr(field, "*") != NULL){
			int bufSize = Buffer.getBufferSize();

			File.seekg(bufSize, ios::cur);
			return -2;

		}else{

			if(recaddr == -1)
				return Buffer.Read(File);
			else
				return Buffer.DRead(File, recaddr);
		}
	}else
		return -1;
}

int BufferFile::Write(int recaddr)
// write the current buffer contents
		{
	File.write("00000", 5);
	if(recaddr == -1)
		return Buffer.Write(File);
	else
		return Buffer.DWrite(File, recaddr);
}

int BufferFile::Append()
// first, write the current buffer at the deleted record
// if the deleted record can't be found
// write the current buffer at the end of File
{
	Rewind();

	if(prevDeleted != 0){
		int recaddr = prevDeleted;
		File.seekg(recaddr + 1, ios::beg);
		File.read((char*) &prevDeleted, 4);

		WriteHeader();

		File.seekp(recaddr, ios::beg);

	}else{
		File.seekp(0, ios::end);
	}

	File.write("00000", 5);
	int result = Buffer.Write(File);

	Rewind();

	return result;
}

// Access to IOBuffer
FixedLengthBuffer & BufferFile::GetBuffer(){
	return Buffer;
}

int BufferFile::Delete(int fieldNum, const char* field){
	char buf[5];
	int bufSize = Buffer.getBufferSize();
	FixedTextBuffer fixbuf = Buffer.getBuffer();
	int prevAddr, recaddr;
	prevAddr = -1;
	Rewind();

	while(!(File.eof() || File.fail())){
		recaddr = File.tellg();
		if(prevAddr == recaddr){
			return -1;
		}
		prevAddr = recaddr;
		File.read(buf, 5);

		if(strstr(buf, "*") == NULL){
			Buffer.Read(File);

			if(fixbuf.isEqualField(fieldNum, field) == 0){
				File.seekp(recaddr, ios::beg);
				File.write("*", 1);
				File.write((char*) &prevDeleted, 4);
				Buffer.Clear();
				Buffer.Write(File);

				prevDeleted = recaddr;
				WriteHeader();
				Rewind();

				return 1;
			}

			Buffer.Clear();
		}else{
			File.seekg(Buffer.getBufferSize(), ios::cur);
		}
	}

	return -1;
}

// protected methods
int BufferFile::ReadHeader(){
	Buffer.ReadHeader(File);
	File.read((char*) &prevDeleted, 4);
	return File.tellg();
}

int BufferFile::WriteHeader(){
	Buffer.WriteHeader(File);
	File.write((char*) &prevDeleted, 4);
	return File.tellp();
}

