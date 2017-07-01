/*
 * main.h
 *
 *  Created on: 2015. 4. 20.
 *      Author: panda
 */

#ifndef MAIN_H_
#define MAIN_H_

/*
 * �η� F�� �ҽ��ڵ带 �̿��Ͽ� ������ �����Ͽ���.
 * 
 * �޼ҵ� Delete�� Ŭ���� BufferFile�� �߰��Ͽ� �������� ���ڵ��� ������ ����(ã�� �� id�� ã��)
 * ���ڵ尡 Ȱ�� �������� �����Ǿ����� ��ũ�ϵ��� �ϱ� ���ؼ� �ʵ带 �� ���ڵ��� ���ۺκп� �߰�
 * Read �޼ҵ� ����
 * Read�� ���� ���ڵ尡 �����Ǿ��ٸ� �ǵ��� ���ϰų�, ���� Ȱ�� ���ڵ带 �ǵ��ؾ� ��
 * Append �޼ҵ� ����
 * ������ ���ڵ尡 ����� �� �ֵ��� ����
 * ���� ����Ʈ�� ��� �ּҸ� �����ϱ� ���� ���� ����� �ʵ带 �߰�
 * ���� ������ ���ڵ��� �ּҸ� �����ϱ� ���� ������ ������ ���ڵ忡 �ִ� ������ �̿�
 */
using namespace std;
#include <iostream>
#include <cctype>
#include <string>
#include <string.h>
#include "buffile/buffile.h"
#include "buffile/fixlen.h"

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
		int Unpack(IOBuffer &);						//Unpack
		int Pack(IOBuffer &);					//Pack
		int InitBuffer(FixedTextBuffer& buffer);
		void Print() const;							//������� ���
		void Clear();

	protected:
		char id[10];
		char name[10];
		char address[15];
		char enrollment[15];
		char credit_hours[10];

};

//void createRecords(BufferFile buffile);
//void printRecords(BufferFile buffile);
//void deleteRecord(BufferFile buffile);
//void appendRecord(BufferFile buffile, char* id);
#endif /* MAIN_H_ */
