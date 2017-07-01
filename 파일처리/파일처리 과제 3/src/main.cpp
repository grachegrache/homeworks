using namespace std;
#include "main.h"

int main(){
	Student std1, std2, std3, std4;
	CourseRegistration crsReg1, crsReg2, crsReg3, crsReg4;
	DelimTextBuffer delbuf1, delbuf2;
	FixedTextBuffer fixbuf1, fixbuf2;
	LengthTextBuffer lenbuf1, lenbuf2;

	//버퍼 초기화
	std1.InitBuffer(fixbuf1);
	crsReg1.InitBuffer(fixbuf2);

	BufferFile delbuffile1((IOBuffer&) delbuf1), delbuffile2((IOBuffer&) delbuf2);
	BufferFile fixbuffile1((IOBuffer&) fixbuf1), fixbuffile2((IOBuffer&) fixbuf2);
	BufferFile lenbuffile1((IOBuffer&) lenbuf1), lenbuffile2((IOBuffer&) lenbuf2);

	//입력
	std1.setAddress("a");
	std1.setCredit_hours("b");
	std1.setEnrollment("c");
	std1.setId("d");
	std1.setName("e");
	crsReg1.setCourseGrade("a");
	crsReg1.setCourseId("b");
	crsReg1.setCredeitHours("c");
	crsReg1.setStudentId("d");
	cout << "Input" << endl;
	cout << "Student" << endl;
	std1.Print();
	cout << "\nCourse Registration" << endl;
	crsReg1.Print();

	//Pack
	std1.Pack(delbuffile1.GetBuffer());
	std1.Pack(fixbuffile1.GetBuffer());
	std1.Pack(lenbuffile1.GetBuffer());
	crsReg1.Pack(delbuffile2.GetBuffer());
	crsReg1.Pack(fixbuffile2.GetBuffer());
	crsReg1.Pack(lenbuffile2.GetBuffer());

	//파일 생성
	delbuffile1.Create("std_delim_buffer.txt");
	fixbuffile1.Create("std_fixed_buffer.txt");
	lenbuffile1.Create("std_length_buffer.txt");
	delbuffile2.Create("crs_delim_buffer.txt");
	fixbuffile2.Create("crs_fixed_buffer.txt");
	lenbuffile2.Create("crs_length_buffer.txt");

	//쓰기
	delbuffile1.Write();
	fixbuffile1.Write();
	lenbuffile1.Write();
	delbuffile2.Write();
	fixbuffile2.Write();
	lenbuffile2.Write();

	//닫기
	delbuffile1.Close();
	fixbuffile1.Close();
	lenbuffile1.Close();
	delbuffile2.Close();
	fixbuffile2.Close();
	lenbuffile2.Close();

	//열기
	delbuffile1.Open("std_delim_buffer.txt");
	fixbuffile1.Open("std_fixed_buffer.txt");
	lenbuffile1.Open("std_length_buffer.txt");
	delbuffile2.Open("crs_delim_buffer.txt");
	fixbuffile2.Open("crs_fixed_buffer.txt");
	lenbuffile2.Open("crs_length_buffer.txt");

	//읽기
	delbuffile1.Read();
	fixbuffile1.Read();
	lenbuffile1.Read();
	delbuffile2.Read();
	fixbuffile2.Read();
	lenbuffile2.Read();

	//Unpack
	std2.Unpack(delbuffile1.GetBuffer());
	std3.Unpack(fixbuffile1.GetBuffer());
	std4.Unpack(lenbuffile1.GetBuffer());
	crsReg2.Unpack(delbuffile2.GetBuffer());
	crsReg3.Unpack(fixbuffile2.GetBuffer());
	crsReg4.Unpack(lenbuffile2.GetBuffer());

	//출력
	cout << "\nOutput" << endl;
	cout << "Student" << endl;
	cout << "Delim Text Buffer" << endl;
	std2.Print();
	cout << "\nFixed Text Buffer" << endl;
	std3.Print();
	cout << "\nLength Text Buffer" << endl;
	std4.Print();

	cout << "\nCourse Registration" << endl;
	cout << "Delim Text Buffer" << endl;
	crsReg2.Print();
	cout << "\nFixed Text Buffer" << endl;
	crsReg3.Print();
	cout << "\nLength Text Buffer" << endl;
	crsReg4.Print();

	return 0;
}
