//============================================================================
// Name        : std_manager.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

using namespace std;
#include "main.h"

int main(){
	cout << "학생 과목 정보 관리 시스템\n" << endl;
	int mode = 0;
	vector < Student > std_list;
	vector < CourseRegistration > crs_list;

	while(cin){
		string input;

		switch(mode){
			case 0:
				cout << "1.Student 2.CourseRegistration 끝내려면 end 입력" << endl;
				cin >> input;
				if(input == "1" || input == "2")
					mode = stoi(input);
				break;
			case 1:
				process < Student > (mode, std_list);
				break;
			case 2:
				process < CourseRegistration > (mode, crs_list);
				break;
			default:
				break;
		}

		if(input == "end")
			break;
	}
	return 0;
}

template<typename T>
void readFromKbd(vector<T>& list){
	T* t = new T;
	t->readFromKbd();
	list.push_back(*t);
	delete t;
}

template<typename T>
void readFromFile(vector<T>& list){
	string reply;
	cout << "현재 리스트가 사라집니다. 계속할까요?(Y 입력)" << endl;
	cin >> reply;
	if(reply == "Y" || reply == "y"){
		cout << "파일명을 입력하세요" << endl;
		cin >> reply;
		list = readListFromFile < T > (reply);
	}
}

template<typename T>
vector<T> readListFromFile(const string file_name){
	vector<T> vec;
	fstream file;
	file.open(file_name, ios::in);
	file.unsetf(ios::skipws);

	while(true){
		T* t = new T;
		if(t->readFromFile(file))
			vec.push_back(*t);
		delete t;

		if(file.fail())
			break;
	}

	file.close();
	return vec;
}

template<typename T>
void writeToFile(const vector<T> list){
	string reply;
	cout << "파일명을 입력하세요" << endl;
	cin >> reply;
	writeListToFile < T > (reply, list);
}

template<typename T>
void writeListToFile(const string file_name, const vector<T> list){
	if(list.empty())
		return;
	fstream file;
	file.open(file_name, ios::out);
	file.unsetf(ios::skipws);

	for(T t : list)
		t.writeToFile(file);

	file.close();
}

template<typename T>
void process(int& mode, vector<T>& list){
	string input, menu_token, lable;

	if(mode == 1){
		menu_token = "학생";
		lable = "ID|NAME|ADDRESS|CREDIT HOURS|ENROLLMENT DATE";
	}else{
		menu_token = "과목";
		lable = "COURSE ID|CREDIT HOURS|COURSE GRADE|STUDENT ID";
	}

	cout << "1." << menu_token << "정보 보기 2.키보드로 " << menu_token << "정보 입력 3.파일로 불러오기 4.파일로 저장하기 5.뒤로" << endl;
	cin >> input;

	if(input == "1"){
		cout << lable << endl;
		for(T t : list)
			cout << t.toString() << endl;
		cout << '\n';
	}else if(input == "2"){
		readFromKbd<T>(list);
	}else if(input == "3"){
		readFromFile<T>(list);
	}else if(input == "4"){
		writeToFile<T>(list);
	}else if(input == "5")
		mode = 0;
}

bool isInt(string str){
	for(int i = 0; i < str.length(); i++)
		if(!isdigit(str[i]))
			return false;

	return true;
}
