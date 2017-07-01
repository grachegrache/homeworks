/*
 * main.h
 *
 *  Created on: 2015. 3. 11.
 *      Author: panda
 */

#ifndef __MAIN_H__
#define __MAIN_H__

// Add function prototypes here
/*
 - 학생과 과목에 대한 정보를 유지하고 처리하는 시스템
 - Student 클래스를 설계: 한 학생에 대한 정보
 identifier, name, address, date of first enrollment(등록일자), number of credit hours completed(현재까지 이수학점)
 생성자, = 연산자 오버로딩, 필드 수정 메소드, 이수학점 수 증가 메소드
 - CourseRegistration 클래스 설계: 한 과목에서 한 학생의 등록 정보
 course identifier, student identifier, number of credit hours, course grade
 적절한 메소드 포함
 - 학생 및 과목 등록 정보 리스트 생성

 입출력 메소드 추가
 - Student  클래스
 입력 파일로부터 학생의 정보를 읽는 메소드 추가
 학생 정보를 출력 파일에 쓰는 메소드 추가(포맷화하여)
 키보드로부터 학생의 정보를 읽는 메소드 추가
 - CourseRegistration 클래스
 입력 파일로부터 과목 등록 정보를 읽는 메소드 추가
 과목 등록 정보를 출력 파일에 쓰는 메소드 추가(포맷화하여)
 키보드로부터 과목 등록 정보를 읽는 메소드 추가
 - 클래스 구현 테스트를 위한 main 함수 추가
 */
using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

class Student{
	public:
		Student();
		~Student();
		Student& operator =(const Student& std);
		void setId(const int num);
		void setName(const char* str);
		void setAddress(const char* str);
		void setEnrollment(const char* str);
		void addCreditHours(const int adds);
		string toString();
		bool readFromFile(fstream& file);
		void readFromKbd();
		void writeToFile(fstream& file);
	protected:
		int id;
		string name;
		string address;
		string enrollment;
		int credit_hours;

};

class CourseRegistration{
	public:
		CourseRegistration();
		~CourseRegistration();
		void setCourseId(const int id);
		void setStudentId(const int id);
		void setCredeitHours(const int credit);
		void setCourseGrade(const int grade);
		bool readFromFile(fstream& file);
		void readFromKbd();
		void writeToFile(fstream& file);
		string toString();
	protected:
		int crs_id;
		int std_id;
		int credit_hours;
		int crs_grade;
};

template<typename T>
void readFromKbd(vector<T>& list);

template<typename T>
void readFromFile(vector<T>& list);
template<typename T>
vector<T> readListFromFile(const string file);

template<typename T>
void writeToFile(const vector<T> list);
template<typename T>
void writeListToFile(const string file_name, const vector<T> list);

template<typename T>
void process(int& mode, vector<T>& list);

bool isInt(string str);
#endif

