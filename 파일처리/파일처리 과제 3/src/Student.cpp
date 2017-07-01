#include "main.h"
using namespace std;

Student::Student(){
	id[0] = 0;
	name[0] = 0;
	address[0] = 0;
	credit_hours[0] = 0;
	enrollment[0] = 0;

}

Student::~Student(){
	id[0] = 0;
	name[0] = 0;
	address[0] = 0;
	credit_hours[0] = 0;
	enrollment[0] = 0;
}

Student& Student::operator =(const Student& std){
	setId(std.id);
	setCredit_hours(std.credit_hours);
	setName(std.name);
	setAddress(std.address);
	setEnrollment(std.enrollment);
}

void Student::setId(const char* str){
	strcpy(id, str);
}

void Student::setName(const char* str){
	strcpy(name, str);
}

void Student::setAddress(const char* str){
	strcpy(address, str);
}

void Student::setEnrollment(const char* str){
	strcpy(enrollment, str);
}

void Student::setCredit_hours(const char* str){
	strcpy(credit_hours, str);
}

int Student::Pack(IOBuffer& buffer) const{
	int result;
	buffer.Clear();
	result = buffer.Pack(id);
	result = result && buffer.Pack(name);
	result = result && buffer.Pack(address);
	result = result && buffer.Pack(enrollment);
	result = result && buffer.Pack(credit_hours);
	return result;
}

int Student::Unpack(IOBuffer& buffer){
	int result;
	result = buffer.Unpack(id);
	result = result && buffer.Unpack(name);
	result = result && buffer.Unpack(address);
	result = result && buffer.Unpack(enrollment);
	result = result && buffer.Unpack(credit_hours);
	return result;
}

int Student::InitBuffer(FixedTextBuffer& buffer){
	int result;
	result = buffer.AddField(10); // id
	result = result && buffer.AddField(10); // name
	result = result && buffer.AddField(15); // address
	result = result && buffer.AddField(15); // enrollment
	result = result && buffer.AddField(2);  // credit hours
	return result;
}

void Student::Print() const{
	cout << "id:" << '\t' << id << endl;
	cout << "name:" << '\t' << name << endl;
	cout << "address:" << '\t' << address << endl;
	cout << "enrollment:" << '\t' << enrollment << endl;
	cout << "credit hours:" << '\t' << credit_hours << endl;
}
