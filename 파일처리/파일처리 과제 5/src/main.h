/*
 * main.h
 *
 *  Created on: 2015. 5. 8.
 *      Author: panda
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <cctype>
#include "recfile/delim.h"
#include "recfile/recfile.h"
#include "recfile/recordng.h"
#include "recfile/textind.h"
#include "recfile/tindbuff.h"
#include "secind.h"
#include "sindbuff.h"

/*
 * ����Ű �ε��� Ŭ���� ���� �� �׽�Ʈ
 7.6���� SecondaryIndex Ŭ������ ����
 ����Ű�� �۰ �ʵ� ���
 �׽�Ʈ�� ���� ���α׷��� �ۼ�
 �������� ���ڵ� ���� ����
 
 �⺻Ű �ε��� ���� ����
 ����Ű �ε��� ���� ����
 �⺻Ű�� ���ڵ� �˻�
 ����Ű�� ���ڵ� �˻�(����Ű -> �⺻Ű -> ���ڵ� Ȯ��)
 */

int RetrieveRecording (Recording & recording, char * key, TextIndex & RecordingIndex, BufferFile & RecordingFile);
int IndexRecordingFile (char * myfile, TextIndex & RecordingIndex);
int SearchOnSecondary(char * composer, TextIndex& index, SecondaryIndex& index2, BufferFile& RecordingFile);

#endif /* MAIN_H_ */
