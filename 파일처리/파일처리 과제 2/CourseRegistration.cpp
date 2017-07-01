/*
 * CourseRegistration.cpp

 *
 *  Created on: 2015. 3. 12.
 *      Author: panda
 */
#include "main.h"
using namespace std;

CourseRegistration::CourseRegistration(){
	crs_id = 0;
	std_id = 0;
	credit_hours = 0;
	crs_grade = 0;
}

CourseRegistration::~CourseRegistration(){
	crs_id = 0;
	std_id = 0;
	credit_hours = 0;
	crs_grade = 0;
}

void CourseRegistration::setCourseGrade(const int grade){
	crs_grade = grade;
}

void CourseRegistration::setCredeitHours(const int credit){
	credit_hours = credit;
}

string CourseRegistration::toString(){
	return to_string(crs_id) + "\t" + to_string(credit_hours) + "\t" + to_string(crs_grade) + "\t" + to_string(std_id);
}

bool CourseRegistration::readFromFile(fstream& file){
	string line;
	char buffer[100] = "";
	file.clear();
	file.getline(buffer, 100);
	line = buffer;
	int prev = 0, index = 0;

	if(buffer[0] != 0){
		index = line.find('\t', prev);
		crs_id = stoi(line.substr(prev, index - prev + 1));
		prev = index + 1;

		index = line.find('\t', prev);
		credit_hours = stoi(line.substr(prev, index - prev + 1));
		prev = index + 1;

		index = line.find('\t', prev);
		crs_grade = stoi(line.substr(prev, index - prev + 1));
		prev = index + 1;

		std_id = stoi(line.substr(prev, line.length() - prev));

		return true;
	}else
		return false;
}

void CourseRegistration::readFromKbd(){
	string input;

	do{
		cout << "COURSE ID" << endl;
		cin >> input;
	}while(isInt(input) == false);
	crs_id = stoi(input);

	do{
		cout << "CREDIT HOURS" << endl;
		cin >> input;
	}while(isInt(input) == false);
	credit_hours = stoi(input);

	do{
		cout << "COURSE GRADE" << endl;
		cin >> input;
	}while(isInt(input) == false);
	crs_grade = stoi(input);

	do{
		cout << "STUDENT ID" << endl;
		cin >> input;
	}while(isInt(input) == false);
	std_id = stoi(input);
}

void CourseRegistration::writeToFile(fstream& file){
	file << toString() + "\n";
}
