/*
 * ȭ�ϸ� : my_assembler.c 
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 *
 */

/*
 *
 * ���α׷��� ����� �����Ѵ�. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * my_assembler ���α׷��� �������� �����͵��� ���ǵ� ���̴�. 
 */

#include "my_assembler.h"

/* -----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ���� 
 * ��ȯ : ���� = 0, ���� = < 0 
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�. 
 *		   ���� �߰������� �������� �ʴ´�. 
 * -----------------------------------------------------------------------------------
 */

int main(int args, char *arg[]){
	if(init_my_assembler() < 0){
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}

	if(assem_pass1() < 0){
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
	if(assem_pass2() < 0){
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}

	//make_output("output") ; 
}
/* -----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� 
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���. 
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
 * ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�. 
 *		   �н�1������..
 *		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
 *		   ���̺��� �����Ѵ�. 
 * 
 * �Ű� : ����
 * ��ȯ : ���� ���� = 0 , ���� = < 0 
 * ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
 *	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�. 
 *		
 * -----------------------------------------------------------------------------------
 */

static int assem_pass1(void){
	//input_data�� ������� token_table�� �Ľ�
	int i = 0;
	for(; i < line_num; i++)
		if(token_parsing(i) < 0)
			return -1;
	return 0;
}

/* -----------------------------------------------------------------------------------
 * ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�. 
 *		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�. 
 *		   ������ ���� �۾��� ����Ǿ� ����. 
 *		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0, �����߻� = < 0 
 * ���� : 
 * -----------------------------------------------------------------------------------
 */

static int assem_pass2(void){
	make_objectcode (NULL);
	return 0;
}
/* -----------------------------------------------------------------------------------
 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)�� 
 *        �����ϴ� �Լ��̴�. 
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : ���� ������� ������ ������ ����. 
 *	
 *	===============================================================================
 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
 *	===============================================================================	   
 *		
 * -----------------------------------------------------------------------------------
 */

int init_inst_file(char *inst_file){
	char inst_line[MAX_INST];
	FILE* fp = NULL;
	//���� ����
	if((fp = fopen(inst_file, "r")) == NULL)
		return -1;

	//�ε��� �ʱ�ȭ
	inst_index = 0;
	int i, last_index, count;

	//�� �پ� �Է¹޾� �Ľ�
	while(fscanf(fp, "%[^\n]\n", inst_line) > 0){
		//���� �ʱ�ȭ
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

	//���� �ݱ�
	fclose(fp);
	return 0;
}

/* -----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о���� �Լ��̴�. 
 * �Ű� : ������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0  
 * ���� : 
 *		
 * -----------------------------------------------------------------------------------
 */

int init_input_file(char *input_file){
	char input_line[1000];
	FILE* fp = NULL;
	//���� ����
	if((fp = fopen(input_file, "r")) == NULL)
		return -1;

	//line_num �ʱ�ȭ
	line_num = 0;

	//�ּ� . �� �����ϰ� �� �� �� �Է¹���
	while(fscanf(fp, "%[^\r]\r\n", input_line) > 0){
		if(input_line[0] != '.')
			input_data[line_num++] = strdup(input_line);
	}

	//���� �ݱ�
	fclose(fp);
	return 0;
}

/* -----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. 
 *        �н� 1�� ���� ȣ��ȴ�. 
 * �Ű� : �ҽ��ڵ��� ���ι�ȣ  
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�. 
 * -----------------------------------------------------------------------------------
 */

int token_parsing(int index){
	//���� �ʱ�ȭ
	int i, last_index, operand_index, tab_count;
	i = 0;
	last_index = 0;
	operand_index = 0;
	tab_count = 0;
	token* line = malloc(sizeof(token));

	//�ҽ� �� ������ �ǹ��ڸ� �������� �Ľ�
	for(; i < strlen(input_data[index]); i++)
		if(tab_count == 0 || tab_count == 1){
			//label, operator�� ���
			if(input_data[index][i] == '\t'){
				char* dest = strndup(&input_data[index][last_index], i - last_index);

				//ù �Ľ��� ��� ��ū�� START, END, OPCODE �� �� �ϳ��̸� operator�� ������
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
			//operand�� ���
			if(input_data[index][i] == '\t' || input_data[index][i] == ','){
				line->operand[operand_index++] = strndup(&input_data[index][last_index], i - last_index);
				last_index = i + 1;

				if(input_data[index][i] == '\t'){
					if(input_data[index][last_index] == '\t'){
						//ù ��° �� ���� ���� ���ڰ� �� �����̸� �ǳʶ�
						last_index++;
						i++;
					}
					tab_count++;
				}
			}
		}else if(tab_count > 3)
			return -1;

	if(tab_count == 2){
		//������ operand�� ������� ����
		line->operand[operand_index] = strndup(&input_data[index][last_index], i - last_index);
	}else if(tab_count == 3){
		//operand ������ �� ���ڰ� ���� => �ּ��� ������
		line->comment = strndup(&input_data[index][last_index], i - last_index);
	}
	token_table[index] = line;

	return 0;
}

/* -----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. 
 * �Ű� : ��ū ������ ���е� ���ڿ� 
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0 
 * ���� : 
 *		
 * -----------------------------------------------------------------------------------
 */

int search_opcode(char *str){
	//�Է��� NULL�̸� ���� ��ȯ
	if(str == 0)
		return -1;

	//OPCODE Ž��
	int i = 0;
	for(; i < MAX_INST; i++){
		if(inst[i][0] == 0)
			return -1;
		else if(strcmp(inst[i][0], str) == 0)
			return i;
	}
}

/* -----------------------------------------------------------------------------------
 * ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�. 
 * �Ű� : ������ ������Ʈ ���ϸ� 
 * ��ȯ : ����
 * ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������ 
 *        ȭ�鿡 ������ش�. 
 *		
 * -----------------------------------------------------------------------------------
 */

void make_objectcode(char *file_name){
	//���� �ʱ�ȭ
	int i, j, opcode_index;
	token* token;
	i = 0;
	j = 0;

	if(file_name != NULL){
		//��� ���Ͽ� ���
		FILE* fp = NULL;

		//������ �����ϴ� ��� ������ ���� ����
		if((fp = fopen(file_name, "w+")) == NULL)
			return;

		//�� ���� ��ū ���
		for(; i < line_num; i++){
			token = token_table[i];

			if(token->label != 0)
				fprintf(fp, "%s", token->label);
			fprintf(fp, "\t%s\t", token->operator);

			for(j = 0; j < MAX_OPERAND; j++){
				if(token->operand[j] != 0){
					if(j != 0){
						//ù operand�� �ƴϸ�
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

		//���� �ݱ�
		fclose(fp);
	}else{
		//�ܼ� â�� ���

		for(; i < line_num; i++){
			token = token_table[i];

			if(token->label != 0)
				printf("%s", token->label);
			printf("\t%s\t", token->operator);

			for(j = 0; j < MAX_OPERAND; j++){
				if(token->operand[j] != 0){
					if(j != 0){
						//ù operand�� �ƴϸ�
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

