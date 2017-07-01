/*
 * main.h
 *
 *  Created on: 2015. 4. 20.
 *      Author: panda
 */

#ifndef MAIN_H_
#define MAIN_H_

/*
 * 부록 F의 소스코드를 이용하여 다음을 구현하여라.
 * 
 * 메소드 Delete를 클래스 BufferFile에 추가하여 고정길이 레코드의 삭제를 구현(찾을 때 id로 찾음)
 * 레코드가 활동 상태인지 삭제되었는지 마크하도록 하기 위해서 필드를 각 레코드의 시작부분에 추가
 * Read 메소드 수정
 * Read는 현재 레코드가 삭제되었다면 판독을 못하거나, 다음 활동 레코드를 판독해야 함
 * Append 메소드 수정
 * 삭제된 레코드가 재사용될 수 있도록 수정
 * 삭제 리스트의 헤드 주소를 저장하기 위해 파일 헤더에 필드를 추가
 * 다음 삭제된 레코드의 주소를 저장하기 위해 각각의 삭제된 레코드에 있는 공간을 이용
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
		Student();										//생성자
		~Student();									//소멸자
		Student& operator =(const Student& std);	//대입 연산자
		void setId(const char* str);
		void setName(const char* str);
		void setAddress(const char* str);
		void setEnrollment(const char* str);
		void setCredit_hours(const char* str);
		int Unpack(IOBuffer &);						//Unpack
		int Pack(IOBuffer &);					//Pack
		int InitBuffer(FixedTextBuffer& buffer);
		void Print() const;							//멤버변수 출력
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
