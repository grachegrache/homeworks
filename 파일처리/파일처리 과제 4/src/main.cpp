/*
 * main.cpp
 *
 *  Created on: 2015. 4. 20.
 *      Author: panda
 */
using namespace std;
#include "main.h"

int main(){
	FixedLengthBuffer fixbuf(100);
	Student std_out, std_in;

	int i, j, id;
	id = 0;
	char id_ptr[3] = {'0', 0, 0};
	BufferFile buffile(fixbuf);

	//버퍼초기화
	std_out.InitBuffer(fixbuf.getBuffer());

	//파일 생성
	buffile.Create("fixed_length_buffer.txt", ios::out);

	//입력
	std_out.setAddress("a");
	std_out.setCredit_hours("b");
	std_out.setEnrollment("c");
	std_out.setName("e");

	//id:0~9 인 레코드 추가
	for(i = 0; i < 10; i++){
		//Pack
		id_ptr[0] = '0' + i;
		std_out.setId(id_ptr);
		std_out.Pack(fixbuf);
		//쓰기
		buffile.Write();

		fixbuf.Clear();
	}

	//닫기
	buffile.Close();

	//열기
	buffile.Open("fixed_length_buffer.txt", ios::in | ios::out);

	for(i = 0; i < 2; i++){
		//Delete
		cout << "\nInput the id to delete" << endl;
		string str;
		cin >> str;
		const char* input = str.c_str();
		buffile.Delete(0, input);

		cout << "\nOutput" << endl;
		cout << "Fixed Length Buffer" << endl;

		while(true){
			//읽기
			if(buffile.Read() == -1)
				break;

			//Unpack
			std_in.Unpack(buffile.GetBuffer());

			//출력
			std_in.Print();

			std_in.Clear();
		}
	}

	//닫기
	buffile.Close();

	//열기
	buffile.Open("fixed_length_buffer.txt", ios::in | ios::out);

	//Append
	id_ptr[0] = '1';
	for(i = 0; i < 2; i++){
		id_ptr[1] = '0' + i;

		std_out.setId(id_ptr);

		//Pack
		std_out.Pack(fixbuf);

		//Append
		buffile.Append();

		fixbuf.Clear();
		cout << "\nOutput" << endl;
		cout << "Fixed Length Buffer" << endl;

		while(true){
			//읽기
			if(buffile.Read() == -1)
				break;

			//Unpack
			std_in.Unpack(buffile.GetBuffer());

			//출력
			std_in.Print();

			std_in.Clear();
		}
	}

	buffile.Close();
}

//void createRecords(BufferFile buffile){
//	Student std_out;
//	FixedLengthBuffer fixbuf = buffile.GetBuffer();
//	char id_ptr[2] = {'1', 0};
//	int i;
//
//	//입력
//	std_out.setAddress("a");
//	std_out.setCredit_hours("b");
//	std_out.setEnrollment("c");
//	std_out.setName("e");
//
//	for(i = 0; i < 10; i++){
//		//Pack
//		id_ptr[0] = '0' + i;
//		std_out.setId(id_ptr);
//		std_out.Pack(fixbuf);
//		//쓰기
//		buffile.Write();
//
//		fixbuf.Clear();
//	}
//
//}

//void printRecords(BufferFile buffile){
//	int j;
//	Student std_in;
//	cout << "\nOutput" << endl;
//	cout << "Fixed Length Buffer" << endl;
//
//	while(true){
//		//읽기
//		if(buffile.Read() == -1)
//			break;
//
//		//Unpack
//		std_in.Unpack(buffile.GetBuffer());
//
//		//출력
//		std_in.Print();
//
//		std_in.Clear();
//	}
//
//}

//void deleteRecord(BufferFile buffile){
//
//	//Delete
//	cout << "\nInput the id to delete" << endl;
//	string str;
//	cin >> str;
//	const char* input = str.c_str();
//	buffile.Delete(0, input);
//
//}

//void appendRecord(BufferFile buffile, char* id){
//	Student std_out;
//	FixedLengthBuffer fixbuf = buffile.GetBuffer();
//	//입력
//	std_out.setAddress("a");
//	std_out.setCredit_hours("b");
//	std_out.setEnrollment("c");
//	std_out.setName("e");
//	std_out.setId(id);
//
////Pack
//	std_out.Pack(fixbuf);
//
////Append
//	buffile.Append();
//
//	fixbuf.Clear();
//
//}
