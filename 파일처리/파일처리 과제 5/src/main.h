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
 * 보조키 인덱스 클래스 구현 및 테스트
 7.6절의 SecondaryIndex 클래스를 구현
 보조키로 작곡가 필드 사용
 테스트를 위한 프로그램을 작성
 가변길이 레코드 파일 생성
 
 기본키 인덱스 파일 생성
 보조키 인덱스 파일 생성
 기본키로 레코드 검색
 보조키로 레코드 검색(보조키 -> 기본키 -> 레코드 확인)
 */

int RetrieveRecording (Recording & recording, char * key, TextIndex & RecordingIndex, BufferFile & RecordingFile);
int IndexRecordingFile (char * myfile, TextIndex & RecordingIndex);
int SearchOnSecondary(char * composer, TextIndex& index, SecondaryIndex& index2, BufferFile& RecordingFile);

#endif /* MAIN_H_ */
