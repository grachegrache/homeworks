/*
 * CourseRegistration.cpp

 *
 *  Created on: 2015. 3. 12.
 *      Author: panda
 */
#include "main.h"
using namespace std;

CourseRegistration::CourseRegistration(){
	crs_id[0] = 0;
	std_id[0] = 0;
	credit_hours[0] = 0;
	crs_grade[0] = 0;
}

CourseRegistration::~CourseRegistration(){
	crs_id[0] = 0;
	std_id[0] = 0;
	credit_hours[0] = 0;
	crs_grade[0] = 0;
}

void CourseRegistration::setCourseGrade(const char* str){
	strcpy(crs_grade, str);
}

void CourseRegistration::setCredeitHours(const char* str){
	strcpy(credit_hours, str);
}

void CourseRegistration::setCourseId(const char* str){
	strcpy(crs_id, str);
}

void CourseRegistration::setStudentId(const char* str){
	strcpy(std_id, str);
}

int CourseRegistration::Pack(IOBuffer& buffer) const{
	int result;
	buffer.Clear();
	result = buffer.Pack(crs_grade);
	result = result && buffer.Pack(crs_id);
	result = result && buffer.Pack(std_id);
	result = result && buffer.Pack(credit_hours);
	return result;
}

int CourseRegistration::Unpack(IOBuffer& buffer){
	int result;
	result = buffer.Unpack(crs_grade);
	result = result && buffer.Unpack(crs_id);
	result = result && buffer.Unpack(std_id);
	result = result && buffer.Unpack(credit_hours);
	return result;
}

int CourseRegistration::InitBuffer(FixedTextBuffer& buffer){
	int result;
	result = buffer.AddField(2); // course grade
	result = result && buffer.AddField(10); // course id
	result = result && buffer.AddField(10); // student id
	result = result && buffer.AddField(2);  // credit hours
	return result;
}

void CourseRegistration::Print() const{
	cout << "course id:" << '\t' << crs_id << endl;
	cout << "course grade:" << '\t' << crs_grade << endl;
	cout << "student id:" << '\t' << std_id << endl;
	cout << "credit hours:" << '\t' << credit_hours << endl;
}
