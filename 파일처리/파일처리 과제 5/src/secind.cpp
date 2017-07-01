/*
 * secind.cpp
 *
 *  Created on: 2015. 5. 8.
 *      Author: panda
 */
#include "main.h"

SecondaryIndex::SecondaryIndex(int maxKeys)
		: NumKeys(0), secondaryKeys(0), primaryKeys(0){
	Init(maxKeys);
}

SecondaryIndex::~SecondaryIndex(){
	delete secondaryKeys;
	delete primaryKeys;
}

int SecondaryIndex::Insert(char* secondaryKey, char* primaryKey){
	int i;
	if(NumKeys == MaxKeys)
		return 0; //no room for another key
	for(i = NumKeys - 1; i >= 0; i--){
		if(strcmp(secondaryKey, secondaryKeys[i]) > 0)
			break;
		else if(strcmp(secondaryKey, secondaryKeys[i]) == 0)
			if(strcmp(primaryKey, primaryKeys[i]) > 0)
				break; // insert into location i+1
		secondaryKeys[i + 1] = secondaryKeys[i];
		primaryKeys[i + 1] = primaryKeys[i];
	}
	secondaryKeys[i + 1] = strdup(secondaryKey);
	primaryKeys[i + 1] = strdup(primaryKey);
	NumKeys++;
	return 1;
}

int SecondaryIndex::Search(char* secondaryKey, int start){
	for(int i = start; i < NumKeys; i++)
		if(strcmp(secondaryKeys[i], secondaryKey) == 0)
			return i; // key found
		else if(strcmp(secondaryKeys[i], secondaryKey) > 0)
			return -1; // not found
	return -1; // not found
}

char* SecondaryIndex::GetPrimaryKey(int index){
	return primaryKeys[index];
}

int SecondaryIndex::Init(int maxKeys){
	if(maxKeys <= 0){
		MaxKeys = 0;
		return 0;
	}
	MaxKeys = maxKeys;
	secondaryKeys = new char*[maxKeys];
	primaryKeys = new char*[maxKeys];
	return 1;
}

int SearchOnSecondary(char* composer, TextIndex& index, SecondaryIndex& index2,
		BufferFile & RecordingFile){
	Recording rec;
	int keyIndex = -1;

	// 화일을 판독하기 위해 기본 키 인덱스를 사용한다.
	do{
		keyIndex++;
		keyIndex = index2.Search(composer, keyIndex);
		if(keyIndex != -1){
			RetrieveRecording(rec, index2.GetPrimaryKey(keyIndex), index, RecordingFile);
			cout << "Found record: " << rec << endl;
		}
	}while(keyIndex != -1);
}

int IndexSecondaryRecordingFile(char* file, SecondaryIndex& recordIndex){
	Recording rec;
	int recaddr, result;
	DelimFieldBuffer Buffer; // create a buffer
	BufferFile RecordingFile(Buffer);
	result = RecordingFile.Open(file, ios::in);
	if(!result){
		cout << "Unable to open file " << file << endl;
		return 0;
	}

	while(1) // loop until the read fails
	{
		recaddr = RecordingFile.Read(); // read next record
		if(recaddr < 0)
			break;
		rec.Unpack(Buffer);
		int index = recordIndex.Insert(rec.Composer, rec.Key());
		cout << recaddr << '\t' << rec << endl;
	}
}
