using namespace std;

#include "buffile.h"

BufferFile::BufferFile(IOBuffer & from)
		: Buffer(from) // create with a buffer
{

}

int BufferFile::Open(char * filename)
// open an existing file and check the header
// a correct header must be on the file
// use ios::nocreate to ensure that a file exists
		{
	// these modes are not allowed when opening an existing file
	if(File.is_open())
		File.close();

	File.open(filename, ios_base::in | ios_base::out);
	if(!File.is_open() || !File.good())
		return FALSE;

	File.seekg(0, ios::beg);
	File.seekp(0, ios::beg);

	return File.good();
}

int BufferFile::Create(char * filename)
// create a new file and write a header on it.
// use ios::nocreate to ensure that no file exists
		{

	if(File.is_open())
		File.close();

	File.open(filename);
	if(!File.is_open())
		File.open(filename, ios_base::in | ios_base::out | ios_base::trunc);

	if(!File.good()){
		File.close();
		return FALSE;
	}

	File.seekg(0, ios::beg);
	File.seekp(0, ios::beg);
	return true;
}

int BufferFile::Close(){
	File.close();
	return TRUE;
}

int BufferFile::Rewind(){
	File.seekg(0, ios::beg);
	File.seekp(0, ios::beg);
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
	if(recaddr == -1)
		return Buffer.Read(File);
//	else
//		return Buffer.DRead(File, recaddr);
	return recaddr;
}

int BufferFile::Write(int recaddr)
// write the current buffer contents
		{
	if(recaddr == -1)
		return Buffer.Write(File);
//	else
//		return Buffer.DWrite(File, recaddr);
	return recaddr;
}

int BufferFile::Append()
// write the current buffer at the end of File
{
	File.seekp(0, ios::end);
	return Buffer.Write(File);
}

// Access to IOBuffer
IOBuffer & BufferFile::GetBuffer(){
	return Buffer;
}

//// protected methods
//int BufferFile::ReadHeader ()
//{
//	return Buffer . ReadHeader (File);	
//}
//
//int BufferFile::WriteHeader ()
//{
//	return Buffer . WriteHeader (File);
//}

