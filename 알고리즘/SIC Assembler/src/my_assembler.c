/*
 * 화일명 : my_assembler.c 
 * 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
 * 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
 *
 */

/*
 *
 * 프로그램의 헤더를 정의한다. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * my_assembler 프로그램의 의존적인 데이터들이 정의된 곳이다. 
 */

#include "my_assembler.h"

/* -----------------------------------------------------------------------------------
 * 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
 * 매계 : 실행 파일, 어셈블리 파일 
 * 반환 : 성공 = 0, 실패 = < 0 
 * 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다. 
 *		   또한 중간파일을 생성하지 않는다. 
 * -----------------------------------------------------------------------------------
 */

int main(int args, char *arg[]){
	if(init_my_assembler() < 0){
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n");
		return -1;
	}

	if(assem_pass1() < 0){
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n");
		return -1;
	}
	if(assem_pass2() < 0){
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n");
		return -1;
	}

	//make_output("output") ; 
}
/* -----------------------------------------------------------------------------------
 * 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0 , 에러 발생 = -1
 * 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기 
 *		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
 *		   구현하였다. 
 * -----------------------------------------------------------------------------------
 */

int init_my_assembler(void){
	int result;

	if((result = init_inst_file("inst.data")) < 0)
		return -1;
	if((result = init_input_file("input.txt")) < 0)
		return -1;
	return result;
}

/* -----------------------------------------------------------------------------------
 * 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다. 
 *		   패스1에서는..
 *		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
 *		   테이블을 생성한다. 
 * 
 * 매계 : 없음
 * 반환 : 정상 종료 = 0 , 에러 = < 0 
 * 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
 *	  따라서 에러에 대한 검사 루틴을 추가해야 한다. 
 *		
 * -----------------------------------------------------------------------------------
 */

static int assem_pass1(void){
	//input_data를 기반으로 token_table에 파싱
	int i = 0;
	for(; i < line_num; i++)
		if(token_parsing(i) < 0)
			return -1;
	return 0;
}

/* -----------------------------------------------------------------------------------
 * 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다. 
 *		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다. 
 *		   다음과 같은 작업이 수행되어 진다. 
 *		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0, 에러발생 = < 0 
 * 주의 : 
 * -----------------------------------------------------------------------------------
 */

static int assem_pass2(void){
	make_objectcode (NULL);
	return 0;
}
/* -----------------------------------------------------------------------------------
 * 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을 
 *        생성하는 함수이다. 
 * 매계 : 기계어 목록 파일
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : 기계어 목록파일 형식은 다음과 같다. 
 *	
 *	===============================================================================
 *		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
 *	===============================================================================	   
 *		
 * -----------------------------------------------------------------------------------
 */

int init_inst_file(char *inst_file){
	char inst_line[MAX_INST];
	FILE* fp = NULL;
	//파일 열기
	if((fp = fopen(inst_file, "r")) == NULL)
		return -1;

	//인덱스 초기화
	inst_index = 0;
	int i, last_index, count;

	//한 줄씩 입력받아 파싱
	while(fscanf(fp, "%[^\n]\n", inst_line) > 0){
		//변수 초기화
		count = 0;
		i = 1;
		last_index = 1;

		for(; i < strlen(inst_line); i++){
			if(inst_line[i] == '|'){
				inst[inst_index][count++] = strndup(&inst_line[last_index], i - last_index);
				last_index = i + 1;
			}
			if(count >= MAX_COLUMNS)
				break;
		}
		inst_index++;
	}

	//파일 닫기
	fclose(fp);
	return 0;
}

/* -----------------------------------------------------------------------------------
 * 설명 : 어셈블리 할 소스코드를 읽어오는 함수이다. 
 * 매계 : 어셈블리할 소스파일명
 * 반환 : 정상종료 = 0 , 에러 < 0  
 * 주의 : 
 *		
 * -----------------------------------------------------------------------------------
 */

int init_input_file(char *input_file){
	char input_line[1000];
	FILE* fp = NULL;
	//파일 열기
	if((fp = fopen(input_file, "r")) == NULL)
		return -1;

	//line_num 초기화
	line_num = 0;

	//주석 . 은 제외하고 한 줄 씩 입력받음
	while(fscanf(fp, "%[^\r]\r\n", input_line) > 0){
		if(input_line[0] != '.')
			input_data[line_num++] = strdup(input_line);
	}

	//파일 닫기
	fclose(fp);
	return 0;
}

/* -----------------------------------------------------------------------------------
 * 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다. 
 *        패스 1로 부터 호출된다. 
 * 매계 : 소스코드의 라인번호  
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다. 
 * -----------------------------------------------------------------------------------
 */

int token_parsing(int index){
	//변수 초기화
	int i, last_index, operand_index, tab_count;
	i = 0;
	last_index = 0;
	operand_index = 0;
	tab_count = 0;
	token* line = malloc(sizeof(token));

	//소스 줄 끝까지 탭문자를 기준으로 파싱
	for(; i < strlen(input_data[index]); i++)
		if(tab_count == 0 || tab_count == 1){
			//label, operator의 경우
			if(input_data[index][i] == '\t'){
				char* dest = strndup(&input_data[index][last_index], i - last_index);

				//첫 파싱인 경우 토큰이 START, END, OPCODE 셋 중 하나이면 operator로 저장함
				if(tab_count == 0 && (search_opcode(dest) != -1 || strcmp(dest, "START") == 0 || strcmp(dest, "END") == 0))
					tab_count = 1;

				switch(tab_count){
					case 0:
						line->label = dest;
						break;
					case 1:
						line->operator =
						dest;
						break;
					default:
						break;
				}
				tab_count++;
				last_index = i + 1;
			}
		}else if(tab_count == 2){
			//operand의 경우
			if(input_data[index][i] == '\t' || input_data[index][i] == ','){
				line->operand[operand_index++] = strndup(&input_data[index][last_index], i - last_index);
				last_index = i + 1;

				if(input_data[index][i] == '\t'){
					if(input_data[index][last_index] == '\t'){
						//첫 번째 탭 문자 다음 문자가 탭 문자이면 건너뜀
						last_index++;
						i++;
					}
					tab_count++;
				}
			}
		}else if(tab_count > 3)
			return -1;

	if(tab_count == 2){
		//마지막 operand가 저장되지 않음
		line->operand[operand_index] = strndup(&input_data[index][last_index], i - last_index);
	}else if(tab_count == 3){
		//operand 다음에 탭 문자가 존재 => 주석이 존재함
		line->comment = strndup(&input_data[index][last_index], i - last_index);
	}
	token_table[index] = line;

	return 0;
}

/* -----------------------------------------------------------------------------------
 * 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다. 
 * 매계 : 토큰 단위로 구분된 문자열 
 * 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0 
 * 주의 : 
 *		
 * -----------------------------------------------------------------------------------
 */

int search_opcode(char *str){
	//입력이 NULL이면 에러 반환
	if(str == 0)
		return -1;

	//OPCODE 탐색
	int i = 0;
	for(; i < MAX_INST; i++){
		if(inst[i][0] == 0)
			return -1;
		else if(strcmp(inst[i][0], str) == 0)
			return i;
	}
}

/* -----------------------------------------------------------------------------------
 * 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다. 
 * 매계 : 생성할 오브젝트 파일명 
 * 반환 : 없음
 * 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어 
 *        화면에 출력해준다. 
 *		
 * -----------------------------------------------------------------------------------
 */

void make_objectcode(char *file_name){
	//변수 초기화
	int i, j, opcode_index;
	token* token;
	i = 0;
	j = 0;

	if(file_name != NULL){
		//출력 파일에 출력
		FILE* fp = NULL;

		//파일이 존재하는 경우 덮어쓰기로 파일 열기
		if((fp = fopen(file_name, "w+")) == NULL)
			return;

		//줄 별로 토큰 출력
		for(; i < line_num; i++){
			token = token_table[i];

			if(token->label != 0)
				fprintf(fp, "%s", token->label);
			fprintf(fp, "\t%s\t", token->operator);

			for(j = 0; j < MAX_OPERAND; j++){
				if(token->operand[j] != 0){
					if(j != 0){
						//첫 operand가 아니면
						fprintf(fp, ",");
					}
					fprintf(fp, "%s", token->operand[j]);
				}else
					break;
			}

			if((opcode_index = search_opcode(token->operator)) >= 0)
				fprintf(fp, "\t%s", inst[opcode_index][2]);
			fprintf(fp, "\r\n");

		}

		//파일 닫기
		fclose(fp);
	}else{
		//콘솔 창에 출력

		for(; i < line_num; i++){
			token = token_table[i];

			if(token->label != 0)
				printf("%s", token->label);
			printf("\t%s\t", token->operator);

			for(j = 0; j < MAX_OPERAND; j++){
				if(token->operand[j] != 0){
					if(j != 0){
						//첫 operand가 아니면
						printf(",");
					}
					printf("%s", token->operand[j]);
				}else
					break;
			}

			if((opcode_index = search_opcode(token->operator)) >= 0)
				printf("\t%s", inst[opcode_index][2]);
			printf("\r\n");
		}
	}

}

