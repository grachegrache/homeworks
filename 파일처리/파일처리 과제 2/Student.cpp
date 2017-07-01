#include "main.h"
using namespace std;

Student::Student(){
	id = 0;
	name = "";
	address = "";
	credit_hours = 0;
	enrollment = "";

}

Student::~Student(){
	id = 0;
	name = "";
	address = "";
	enrollment = "";
//	name.~basic_string();
//	address.~basic_string();
	credit_hours = 0;
//	enrollment.~basic_string();
}

Student& Student::operator =(const Student& std){
	id = std.id;
	credit_hours = std.credit_hours;
	setName(std.name.c_str());
	setAddress(std.address.c_str());
	setEnrollment(std.enrollment.c_str());
}

void Student::setId(const int num){
	id = num;
}

void Student::setName(const char* str){
	name = str;
}

void Student::setAddress(const char* str){
	address = str;
}

void Student::setEnrollment(const char* str){
	enrollment = str;
}

void Student::addCreditHours(const int adds){
	credit_hours += adds;
}

string Student::toString(){
	return to_string(id) + "\t" + name + "\t" + address + "\t" + to_string(credit_hours) + "\t" + enrollment;
}

bool Student::readFromFile(fstream& file){
	string line;
	char buffer[100] = "";
	file.clear();
	file.getline(buffer, 100);
	line = buffer;
	int prev = 0, index = 0;

	if(buffer[0] != 0){
		index = line.find('\t', prev);
		id = stoi(line.substr(prev, index - prev + 1));
		prev = index + 1;

		index = line.find('\t', prev);
		name = line.substr(prev, index - prev + 1);
		prev = index + 1;

		index = line.find('\t', prev);
		address = line.substr(prev, index - prev + 1);
		prev = index + 1;

		index = line.find('\t', prev);
		credit_hours = stoi(line.substr(prev, index - prev + 1));
		prev = index + 1;

		enrollment = line.substr(prev, line.length() - prev);

		return true;
	}else
		return false;
}

void Student::readFromKbd(){
	string input;

	do{
		cout << "ID" << endl;
		cin >> input;
	}while(isInt(input) == false);
	id = stoi(input);

	cout << "NAME" << endl;
	cin >> input;
	name = input;

	cout << "ADDRESS" << endl;
	cin >> input;
	address = input;

	do{
		cout << "CREDIT HOURS" << endl;
		cin >> input;
	}while(isInt(input) == false);
	credit_hours = stoi(input);

	cout << "ENROLLMENT DATE" << endl;
	cin >> input;
	enrollment = input;
}

void Student::writeToFile(fstream& file){
	file << toString() + "\n";
}
