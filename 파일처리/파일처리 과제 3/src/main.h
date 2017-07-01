/*
 * main.h
 *
 *  Created on: 2015. 3. 11.
 *      Author: panda
 */

#ifndef __MAIN_H__
#define __MAIN_H__

/*
 IOBuffer를 사용하여 파일로부터 객체를 메모리로 로드하고 파일에 객체를 저장하는 메소드 추가
 
 Student  클래스
 Pack과 Unpack 메소드를 추가
 학생 레코드의 파일을 생성하기 위해 BufferFile 클래스를 사용
 IOBuffer 클래스에 의해서 제공되는 버퍼의 타입을 사용해서 메소드를 테스트
 
 CourseRegistration 클래스
 Pack과 Unpack 메소드를 추가
 수강 등록 파일을 생성하기 위해 BufferFile 클래스를 사용
 IOBuffer 클래스에 의해 제공되는 버퍼의 타입을 사용해서 메소드를 테스트
 */
using namespace std;
//#include <fstream>
//#include <cctype>
#include <string.h>
#include <typeinfo>
#include "buffile.h"
#include "deltext.h"
#include "fixtext.h"
#include "lentext.h"

class Student{
	public:
		Student();										//생성자
		~Student();									//소멸자
		Student& operator =(const Student& std);	//대입 연산자
		void setId(const char* str);
		void setName(const char* str);
		void setAddress(const char* str);
		void setEnrollment(const char* str);
		void setCredit_hours(const char* str);
		int InitBuffer(FixedTextBuffer &);			//버퍼 초기화
		int Unpack(IOBuffer &);						//Unpack
		int Pack(IOBuffer &) const;					//Pack
		void Print() const;							//멤버변수 출력

	protected:
		char id[10];
		char name[10];
		char address[15];
		char enrollment[15];
		char credit_hours[10];

};

class CourseRegistration{
	public:
		CourseRegistration();						//생성자
		~CourseRegistration();						//소멸자
		void setCourseId(const char* str);
		void setStudentId(const char* str);
		void setCredeitHours(const char* str);
		void setCourseGrade(const char* str);
		int InitBuffer(FixedTextBuffer &);			//버퍼 초기화
		int Unpack(IOBuffer &);						//Unpack
		int Pack(IOBuffer &) const;					//Pack
		void Print() const;							//멤버변수 출력

	protected:
		char crs_id[10];
		char std_id[10];
		char credit_hours[10];
		char crs_grade[10];
};

#endif

