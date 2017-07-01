/*
 * main.cpp
 *
 *  Created on: 2015. 5. 8.
 *      Author: panda
 */
#include "main.h"
using namespace std;

int main(void){
	//가변길이 레코드 파일 생성
	int recaddr;
	DelimFieldBuffer Buffer;
	RecordFile<Recording> recfile(Buffer);
	recfile.Create("record.dat", ios::out);
	Recording* R[10];
	R[0] = new Recording("LON", "2312", "Romeo and Juliet", "Prokofiev", "Maazel");
	R[1] = new Recording("RCA", "2626", "Quartet in C Sharp Minor", "Beethoven", "Julliard");
	R[2] = new Recording("WAR", "23699", "Touchstone", "Corea", "Corea");
	R[3] = new Recording("ANG", "3795", "Symphony No. 9", "Beethoven", "Giulini");
	R[4] = new Recording("COL", "38358", "Nebraska", "Springsteen", "Springsteen");
	R[5] = new Recording("DG", "18807", "Symphony No. 9", "Beethoven", "Karajan");
	R[6] = new Recording("MER", "75016", "Coq d'or Suite", "Rimsky-Korsakov", "Leinsdorf");
	R[7] = new Recording("COL", "31809", "Symphony No. 9", "Dvorak", "Bernstein");
	R[8] = new Recording("DG", "139201", "Violin Concerto", "Beethoven", "Ferras");
	R[9] = new Recording("FF", "245", "Good News", "Sweet Honey in the Rock",
			"Sweet Honey in the Rock");
	for(int i = 0; i < 10; i++){
		R[i]->Pack(Buffer);
		recaddr = recfile.Write(*R[i]);
		cout << "Recording R[" << i << "] at recaddr " << recaddr << endl;
		delete R[i];
	}
	recfile.Close();

	cout << endl;
	
	//기본키 인덱스 파일 생성
	TextIndex RecordingIndex(10);
	IndexRecordingFile("record.dat", RecordingIndex);
	// store the index in a file
	TextIndexBuffer IndexBuffer(12, 10); // 12 byte key, 10 keys
	BufferFile IndexFile(IndexBuffer);
	IndexBuffer.Pack(RecordingIndex);
	IndexFile.Create("recindex.dat", ios::out);
	IndexFile.Write();
	IndexFile.Close();

	cout << endl;
	
	//보조키 인덱스 파일 생성
	SecondaryIndex recIndex2(10);
	IndexSecondaryRecordingFile("record.dat", recIndex2);
	// 보조키 저장
	SecondaryIndexBuffer IndexBuffer2(12, 10);
	BufferFile IndexFile2(IndexBuffer2);
	IndexBuffer2.Pack(recIndex2);
	IndexFile2.Create("recindex2.dat", ios::out);
	IndexFile2.Write();
	IndexFile2.Close();

	cout << endl;
	
	//기본키로 검색
	Recording rec;
	DelimFieldBuffer Buffer2; // create a buffer
	BufferFile RecordingFile(Buffer2);
	RecordingFile.Open("record.dat", ios::in);
	RetrieveRecording(rec, "LON2312", RecordingIndex, RecordingFile);
	cout << "Found record: " << rec << endl;

	cout << endl;
	
	//보조키로 검색
	SearchOnSecondary("Beethoven", RecordingIndex, recIndex2, RecordingFile);

	RecordingFile.Close();
}

