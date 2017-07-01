/*
 * main.h
 *
 *  Created on: 2015. 3. 11.
 *      Author: panda
 */

#ifndef __MAIN_H__
#define __MAIN_H__

/*
 IOBuffer�� ����Ͽ� ���Ϸκ��� ��ü�� �޸𸮷� �ε��ϰ� ���Ͽ� ��ü�� �����ϴ� �޼ҵ� �߰�
 
 Student  Ŭ����
 Pack�� Unpack �޼ҵ带 �߰�
 �л� ���ڵ��� ������ �����ϱ� ���� BufferFile Ŭ������ ���
 IOBuffer Ŭ������ ���ؼ� �����Ǵ� ������ Ÿ���� ����ؼ� �޼ҵ带 �׽�Ʈ
 
 CourseRegistration Ŭ����
 Pack�� Unpack �޼ҵ带 �߰�
 ���� ��� ������ �����ϱ� ���� BufferFile Ŭ������ ���
 IOBuffer Ŭ������ ���� �����Ǵ� ������ Ÿ���� ����ؼ� �޼ҵ带 �׽�Ʈ
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
		Student();										//������
		~Student();									//�Ҹ���
		Student& operator =(const Student& std);	//���� ������
		void setId(const char* str);
		void setName(const char* str);
		void setAddress(const char* str);
		void setEnrollment(const char* str);
		void setCredit_hours(const char* str);
		int InitBuffer(FixedTextBuffer &);			//���� �ʱ�ȭ
		int Unpack(IOBuffer &);						//Unpack
		int Pack(IOBuffer &) const;					//Pack
		void Print() const;							//������� ���

	protected:
		char id[10];
		char name[10];
		char address[15];
		char enrollment[15];
		char credit_hours[10];

};

class CourseRegistration{
	public:
		CourseRegistration();						//������
		~CourseRegistration();						//�Ҹ���
		void setCourseId(const char* str);
		void setStudentId(const char* str);
		void setCredeitHours(const char* str);
		void setCourseGrade(const char* str);
		int InitBuffer(FixedTextBuffer &);			//���� �ʱ�ȭ
		int Unpack(IOBuffer &);						//Unpack
		int Pack(IOBuffer &) const;					//Pack
		void Print() const;							//������� ���

	protected:
		char crs_id[10];
		char std_id[10];
		char credit_hours[10];
		char crs_grade[10];
};

#endif

