/*
 * secind.h
 *
 *  Created on: 2015. 5. 9.
 *      Author: panda
 */

#ifndef SECIND_H_
#define SECIND_H_

class SecondaryIndex{
// ���ڵ� ������ ���ڿ��� �ε���
	public:
		SecondaryIndex(int maxKeys = 100);
		~SecondaryIndex();
		int Insert(char* secondaryKey, char* primaryKey);
		int Search(char* secondaryKey, int start = 0); // �⺻ Ű�� �ε����� �ǵ����ش�.
		char* GetPrimaryKey(int index);

	protected:
		int MaxKeys;
		int NumKeys;
		char** secondaryKeys;
		char** primaryKeys;
		int Init(int maxKeys);
		friend class SecondaryIndexBuffer;
};

int IndexSecondaryRecordingFile(char* file, SecondaryIndex& recordIndex);

#endif /* SECIND_H_ */
