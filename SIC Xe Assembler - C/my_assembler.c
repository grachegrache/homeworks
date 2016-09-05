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
#include "hex.h"

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
	if((result = init_input_file(input_file)) < 0)
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
	print_intercode();
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
	int i, k;
	locctr = 0;
	last_ctr = 0;
	m_index = 0;
	k = -1;
	for(i = 0; i < line_num; i++)
		make_intercode(token_table[i], &k);

	make_objectcode(output_file);
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
	program_count = 0;

	//���� ����
	if((fp = fopen(input_file, "r")) == NULL)
		return -1;

	//line_num �ʱ�ȭ
	line_num = 0;

	//�ּ� . �� �����ϰ� �� �� �� �Է¹���
	while(fscanf(fp, "%[^\r]\r\n", input_line) > 0){
		if(input_line[0] != '.'){
			if(strstr(input_line, "START") != NULL)
				program_count++;
			else if(strstr(input_line, "CSECT") != NULL)
				program_count++;

			input_data[line_num++] = strdup(input_line);
		}
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
	line->label = 0;
	line->operand[0] = 0;
	line->operand[1] = 0;
	line->operand[2] = 0;
	line->comment = 0;

	//�ҽ� �� ������ �ǹ��ڸ� �������� �Ľ�
	for(; i < strlen(input_data[index]); i++)
		if(tab_count == 0 || tab_count == 1){
			//label, operator�� ���
			if(input_data[index][i] == '\t'){
				char* dest = strndup(&input_data[index][last_index], i - last_index);

				//ù �Ľ��� ���
				if(tab_count == 0)
					//��ū�� START, END, OPCODE �� �� �ϳ��̸� operator�� ������
					//�Ǵ�, ù ���ڰ� +, #, @�̰ų� ��ū�� EXTDEF, EXTREF, LTORG�̸� operator�� ������
					if(search_opcode(dest) != -1 || ((strcmp(dest, "START") && strcmp(dest, "END") && strcmp(
							dest, "EXTDEF") && strcmp(dest, "EXTREF")) == 0) || dest[0] == '+'){
						line->label = "";
						tab_count = 1;
					}

				switch(tab_count){
					case 0:
						line->label = dest;
						break;
					case 1:
						line->operator = dest;
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
				line->operand[operand_index++] = strndup(&input_data[index][last_index],
						i - last_index);
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

	if(i != last_index)
		//���� �Ľ����� ���� ��ū�� ������
		if(tab_count == 0){
			//LTORG�� ���� �ϳ��� ��ɾ�
			line->operator = strndup(&input_data[index][last_index], i - last_index);

		}else if(tab_count == 1){
			//operator�� ������� �ʾ���
			line->operator = strndup(&input_data[index][last_index], i - last_index);

		}else if(tab_count == 2){
			//������ operand�� ������� �ʾ���
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
	int i, j, k, l, ext_start, ext_index, last_addr, T_size, start, pre_blank;
	token* token;
	k = -1;
	j = 2;

	FILE* fp = NULL;

	if(file_name != NULL){
		//������ �����ϴ� ��� ������ ���� ����
		if((fp = fopen(file_name, "w+")) == NULL)
			return;
	}else
		fp = stdout;

	for(i = 0; i < line_num; i++){
		token = token_table[i];
		if(get_inst_size(token) == 0){
			k++;
			last_addr = 0;
			pre_blank = 0;

			//H ���ڵ� ���
			fprintf(fp, "H%-6s000000%06X\r\n", token->label, program_length[k]);

			//D ���ڵ� ���
			if((k == 0 && def_index[0] != 0) || (k > 0 && def_index[k] > def_index[k - 1])){
				fprintf(fp, "D");

				if(k > 0)
					ext_start = def_index[k - 1];
				else
					ext_start = 0;

				for(l = ext_start; l < def_index[k]; l++){
					ext_index = search_symbol(extdef[l]);
					fprintf(fp, "%-6s%06X", extdef[l], sym_table[ext_index].addr);
				}

				fprintf(fp, "\r\n");
			}

			//R ���ڵ� ���
			if((k == 0 && ref_index[0] != 0) || (k > 0 && ref_index[k] > ref_index[k - 1])){
				fprintf(fp, "R");

				if(k > 0)
					ext_start = ref_index[k - 1];
				else
					ext_start = 0;

				for(l = ext_start; l < ref_index[k]; l++)
					fprintf(fp, "%-6s", extref[l]);

				fprintf(fp, "\r\n");
			}

			//T ���ڵ� ���
			while(pre_blank < 2){
				//����� object code�� ����, ����, �� �ε��� ���ϱ�
				start = j;
				T_size = 0;
				while(T_size <= 60)
					if(object_code[j] == NULL){
						pre_blank++;
						j++;
						break;
					}else{
						pre_blank = 0;

						if(T_size + strlen(object_code[j]) > 60)
							break;
						T_size += strlen(object_code[j]);
						j++;
					}

				if(j - start > 1){
					//�� ���ڵ� ���
					fprintf(fp, "T%06X%02X", last_addr, T_size / 2);
					for(l = start; l < j; l++)
						if(object_code[l] != NULL)
							fprintf(fp, "%s", object_code[l]);
					fprintf(fp, "\r\n");

					last_addr += T_size / 2;
				}
			}

			//M ���ڵ� ���
			for(l = 0; m_table[l].program_index != k && l < m_index; l++)
				;

			while(m_table[l].program_index == k && l < m_index){
				fprintf(fp, "M%06X%02X%c%s\r\n", m_table[l].addr, m_table[l].len, m_table[l].op,
						m_table[l].ref);
				l++;
			}

			//E ���ڵ� ���
			fprintf(fp, "E");
			if(k == 0)
				fprintf(fp, "000000");
			fprintf(fp, "\r\n\r\n");
		}
	}

	if(file_name != NULL)
		//���� �ݱ�
		fclose(fp);
}

/*
 * token �ϳ��� object code�� ����� �Լ� 
 */
void make_intercode(token* token, int* program_count){
	int i, k, opcode_index, symbol_index, format, TA, immediate, reg_num, is_exist, num;
	char* hex = 0;
	char* operator;
	char* operand;
	char* start;
	immediate = 0;
	is_exist = 0;
	k = *program_count;

	format = get_inst_size(token);
	if(format == 0){
		len = 0;
		last_ctr = 0;
		locctr = 0;
		k++;
		object_code[code_index++] = 0;
		object_code[code_index++] = 0;
	}else if(format > 0)
		locctr += format;

//opcode �˻�
	operator = token->operator;
	if(token->operator[0] == '+')
		operator += 1;

//opcode�� ������
	if((opcode_index = search_opcode(operator)) >= 0){

		//���Ŀ� ���� �����Ҵ�
		switch(format){
			case 1:
				object_code[code_index] = calloc(3, sizeof(char));
				break;
			case 2:
				object_code[code_index] = calloc(5, sizeof(char));
				break;
			case 3:
				object_code[code_index] = calloc(7, sizeof(char));
				break;
			case 4:
				object_code[code_index] = calloc(9, sizeof(char));
				break;
			default:
				break;
		}

		if(format > 1)
			strcpy(object_code[code_index], inst[opcode_index][2]);

		if(format > 2){
			object_code[code_index][2] = '0';
			object_code[code_index][3] = 0;
		}else if(format == 2)
			object_code[code_index][2] = 0;

		if(format > 2){
			//#, @(immediate, indirect) �˻�
			operand = token->operand[0];
			num = get_dec(object_code[code_index][1]);

			switch(operand[0]){
				case '#':
					num += I;
					operand += 1;
					immediate = 1;
					break;
				case '@':
					num += N;
					operand += 1;
					break;
				default:
					num += N + I;
					break;
			}
			object_code[code_index][1] = get_hex(num);

			//x, e ã��
			for(i = 1; i < 3; i++)
				if(token->operand[i] != NULL)
					if(strcmp(token->operand[i], "X") == 0){
						is_exist = 1;
						break;
					}

			if(is_exist){
				num = get_dec(object_code[code_index][2]);
				num += op_X;
				object_code[code_index][2] = get_hex(num);
			}
		}
		if(format == 4){
			num = get_dec(object_code[code_index][2]);
			num += E;
			object_code[code_index][2] = get_hex(num);
		}

		//TA �˻�
		if(format > 2){
			if(strcmp(operand, "") == 0){
				TA = -1;
				hex = "000";
			}else if(is_digit(operand)){
				if(immediate)
					hex = get_formated_hex(dectohex(atoi(operand)), 3);
				else
					TA = atoi(operand);
			}else if((symbol_index = search_extref(operand, k)) != -1){
				TA = -1;
				hex = "000";
			}else if((symbol_index = search_symbol(operand)) != -1){
				TA = sym_table[symbol_index].addr;
			}else
				TA = -1;
		}

		//�ּ� ���
		if(immediate == 0){
			switch(format){
				case 2:
					for(i = 0; i < 2; i++){
						if((reg_num = get_reg_num(token->operand[i])) != -1)
							hex = dectohex(reg_num);
						else
							hex = "0";
						strcat(object_code[code_index], hex);
					}
					break;
				case 3:
					if(TA != -1){
						if(strcmp(operator, "RSUB") != 0){
							if(TA - locctr >= 16 * 16 * 16){
								num = get_dec(object_code[code_index][2]);
								num += op_B;
								object_code[code_index][2] = get_hex(num);

							}else{
								num = get_dec(object_code[code_index][2]);
								num += P;
								object_code[code_index][2] = get_hex(num);

								hex = get_formated_hex(dectohex(TA - locctr), 3);
								strcat(object_code[code_index], hex);
							}
						}
					}else if(strcmp(hex, "000") == 0)
						strcat(object_code[code_index], hex);
					break;
				case 4:
					hex = "00000";
					m_table[m_index].program_index = k;
					m_table[m_index].len = 5;
					m_table[m_index].addr = len / 2 + 1;
					m_table[m_index].op = '+';
					m_table[m_index].ref = token->operand[0];
					m_index++;
					strcat(object_code[code_index], hex);
					break;
				default:
					break;
			}
		}else if(hex != 0)
			strcat(object_code[code_index], hex);

	}else if((strcmp(token->operator, "WORD") && strcmp(token->operator, "BYTE")) == 0){
		//opcode�� �ƴϰ� WORD Ȥ�� BYTE�� ��
		if(strcmp(token->operator, "WORD") == 0){
			//���ڷ� �̷���� ���� ������
			if(!is_digit(token->operand[0])){
				char op;
				int index_op, index_op1, index_op2;
				char* op1;
				char* op2;

				//������ ã��
				if(strstr(token->operand[0], "+") != NULL){
					op = '+';
				}else if(strstr(token->operand[0], "-") != NULL){
					op = '-';
				}else if(strstr(token->operand[0], "*") != NULL){
					op = '*';
				}else if(strstr(token->operand[0], "/") != NULL){
					op = '/';
				}

				//������ �ε��� ã��
				for(index_op = 0; index_op < strlen(token->operand[0]); index_op++)
					if(token->operand[0][index_op] == op)
						break;

				//�ǿ����� ����
				op1 = strndup(token->operand[0], index_op);
				op2 = strdup(token->operand[0] + index_op + 1);

				m_table[m_index].program_index = k;
				m_table[m_index].len = 6;
				m_table[m_index].addr = len / 2;
				m_table[m_index].op = '+';
				m_table[m_index].ref = op1;
				m_index++;
				m_table[m_index].program_index = k;
				m_table[m_index].len = 6;
				m_table[m_index].addr = len / 2;
				m_table[m_index].op = op;
				m_table[m_index].ref = op2;
				m_index++;

				hex = "000000";
			}else
				hex = get_formated_hex(dectohex(atoi(token->operand[0])), 6);

		}else if(strcmp(token->operator, "BYTE") == 0){
			start = strstr(token->operand[0], "'") + 1;

			for(i = 0; i < strlen(start); i++)
				if(start[i] == '\'')
					break;

			switch(token->operand[0][0]){
				case 'X':
					hex = strndup(start, i);
					break;
				case 'C':
					hex = calloc(i * 2, sizeof(char));
					hex[0] = 0;
					for(i = 0; i < strlen(start) - 1; i++)
						strcat(hex, dectohex(start[i]));
					break;
				default:
					break;
			}

		}

		object_code[code_index] = hex;

	}

	if(object_code[code_index] != NULL){
		len += strlen(object_code[code_index]);
		//�� �ٿ��� object code�� �Ľ̵��� �ʾ��� ��
		if(locctr - last_ctr != format){
			object_code[code_index + 1] = object_code[code_index];
			object_code[code_index] = 0;
			code_index++;
		}
		last_ctr = locctr;
		code_index++;
	}

//LTORG�� END�� ������ object_code�� ���ͷ� �߰�
	if((strcmp(token->operator, "LTORG") && strcmp(token->operator, "END")) == 0)
		while((len = add_literal()) > 0)
			;

	*program_count = k;
}

/*
 * �߰��ڵ带 �ܼ� â�� ����ϸ�
 * sym_table�� �ɺ��� �����ϰ�
 * LTORG, END�� ������ ���ͷ��� �߰��ϴ� �Լ�
 * 
 */
void print_intercode(){
//�ܼ� â�� ���
	int i, j, k, inst_size, index, opcode_index;
	token* token;
	char* hex;
	locctr = 0;
	index = 0;
	k = -1;
	program_length = calloc(program_count, sizeof(int));
	def_index = calloc(program_count, sizeof(int));
	ref_index = calloc(program_count, sizeof(int));
	def_index[0] = 0;
	ref_index[0] = 0;

	for(i = 0; i < line_num; i++){
		token = token_table[i];

		if((inst_size = get_inst_size(token)) != -1){
			if(inst_size == 0){
				k++;
				if(k > 0){
					program_length[k - 1] = locctr;
					def_index[k] = def_index[k - 1];
					ref_index[k] = ref_index[k - 1];
				}
				locctr = 0;
			}

			hex = get_formated_hex(dectohex(locctr), 4);
			printf("%s", hex);

		}

		//sym_table�� �ɺ� �߰�
		if(token->label != 0){
			if(search_symbol(token->label) == -1){
				if(strcmp(token->operator, "EQU") == 0){
					if(print_equal(token, &sym_table[index]))
						index++;

				}else{
					sym_table[index].symbol[0] = 0;
					strcpy(sym_table[index].symbol, token->label);
					sym_table[index++].addr = locctr;
				}
			}
		}

		if(inst_size > 0)
			locctr += inst_size;

		printf("\t");

		if(token->label != NULL)
			printf("%s", token->label);

		printf("\t%s\t", token->operator);

		//EXTDEF, EXTREF ����
		if(def_index[k] < MAX_LINES)
			if(strcmp(token->operator, "EXTDEF") == 0)
				for(j = 0; j < MAX_OPERAND; j++){
					if(token->operand[j] != NULL)
						extdef[def_index[k]++] = token->operand[j];
				}
		if(ref_index[k] < MAX_LINES)
			if(strcmp(token->operator, "EXTREF") == 0){
				for(j = 0; j < MAX_OPERAND; j++){
					if(token->operand[j] != NULL)
						extref[ref_index[k]++] = token->operand[j];
				}
			}

		//���ͷ� �ɺ� ���̺� �߰�
		if(token->operand[0] != NULL)
			if(token->operand[0][0] == '=')
				if(search_symbol(token->operand[0]) == -1){
					sym_table[index].symbol[0] = 0;
					strcpy(sym_table[index].symbol, token->operand[0]);
					sym_table[index++].addr = -1;

				}

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

		printf("\r\n");

		//LTORG Ȥ�� END�� ������ �� ������ ���� ���ͷ��� �߰���
		if((strcmp(token->operator, "LTORG") && strcmp(token->operator, "END")) == 0)
			print_literal();
	}

	program_length[k] = locctr;
}

void print_literal(){
	int i;
	char* hex;

	for(i = 0; i < MAX_LINES; i++){
		if(sym_table[i].symbol[0] == '=' && sym_table[i].addr == -1){
			sym_table[i].addr = locctr;
			hex = get_formated_hex(dectohex(locctr), 4);
			printf("%s", hex);
			printf("\t*\t%s\r\n", sym_table[i].symbol);
			locctr += get_byte_size(sym_table[i].symbol + 1);
		}
	}
}

int print_equal(token* token, symbol* symbol){
	char* hex;
	char* op1;
	char* op2;
	char op = 0;
	int i, index_op, index_op1, index_op2, result;
	result = -1;

	if(strcmp(token->operand[0], "*") == 0){
		result = locctr;

		//symbol table�� �߰�
		symbol->symbol[0] = 0;
		strcpy(symbol->symbol, token->label);

		symbol->addr = result;
	}else{
		//������ ã��
		if(strstr(token->operand[0], "+") != NULL){
			op = '+';
		}else if(strstr(token->operand[0], "-") != NULL){
			op = '-';
		}else if(strstr(token->operand[0], "*") != NULL){
			op = '*';
		}else if(strstr(token->operand[0], "/") != NULL){
			op = '/';
		}

		if(op == 0)
			return 0;

		//������ �ε��� ã��
		for(index_op = 0; index_op < strlen(token->operand[0]); index_op++)
			if(token->operand[0][index_op] == op)
				break;

		//�ǿ����� ����
		op1 = strndup(token->operand[0], index_op);
		op2 = strdup(token->operand[0] + index_op + 1);

		//�ǿ����� �ε��� ã��
		index_op1 = search_symbol(op1);
		index_op2 = search_symbol(op2);

		free(op1);
		free(op2);

		if(index_op1 == -1 || index_op2 == -1)
			return 0;

		//���
		switch(op){
			case '+':
				result = sym_table[index_op1].addr + sym_table[index_op2].addr;
				break;
			case '-':
				result = sym_table[index_op1].addr - sym_table[index_op2].addr;
				break;
			case '*':
				result = sym_table[index_op1].addr * sym_table[index_op2].addr;
				break;
			case '/':
				result = sym_table[index_op1].addr / sym_table[index_op2].addr;
				break;
			default:
				return 0;
		}

	}

//������� ã��������
	if(result != -1){

		//���
		hex = get_formated_hex(dectohex(result), 4);
		printf("%s", hex);

		return 1;
	}

	return 0;
}

/*
 * ���� locctr���� sym_table�� addr�� ���Ͽ�
 * ���� ���� ���� ���ͷ��� object_code�� �߰��ϴ� �Լ�
 */
int add_literal(){
	int i, size;
	char* code = NULL;

	for(i = 0; i < MAX_LINES; i++){

		if(locctr == sym_table[i].addr)
			if(sym_table[i].symbol[0] == '='){
				char* operand = sym_table[i].symbol + 1;
				char* start;
				start = strstr(operand, "'") + 1;

				for(i = 0; i < strlen(start); i++)
					if(start[i] == '\'')
						break;

				switch(operand[0]){
					case 'X':
						code = strndup(start, i);
						size = i / 2 + i % 2;
						break;
					case 'C':
						code = calloc(i * 2, sizeof(char));
						size = i;

						for(i = 0; i < strlen(start); i++)
							if(start[i] == '\'')
								break;
							else
								strcat(code, dectohex(start[i]));
						break;
					default:
						code = dectohex(atoi(operand[0]));
						i = strlen(code);
						size = i / 2 + i % 2;
						break;
				}

				object_code[code_index] = code;
				locctr += size;
				//�� �ٿ��� object code�� �Ľ̵��� �ʾ��� ��
				if(locctr - last_ctr != size){
					object_code[code_index + 1] = object_code[code_index];
					object_code[code_index] = 0;
					code_index++;
				}
				code_index++;
				return strlen(code);
			}
	}

	return 0;
}

/*
 * symbol_table���� ���� �̸��� symbol�� �����ϸ� index ����
 */
int search_symbol(char* symbol){
	int i;

	if(symbol == NULL)
		return -1;

	for(i = 0; i < MAX_LINES; i++){
		if(strcmp(sym_table[i].symbol, symbol) == 0)
			return i;
	}

	return -1;
}

/*
 * extref ���� ���� �̸��� �ε��� ��ȯ
 */
int search_extref(char* str, int program_index){
	int i, start;

	if(str == NULL)
		return -1;

	if(program_index > 0)
		start = ref_index[program_index - 1];

	for(i = start; i < ref_index[program_index]; i++){
		if(extref[i] != NULL)
			if(strcmp(extref[i], str) == 0)
				return i;
	}

	return -1;
}

int get_reg_num(char* operand){
	if(operand == NULL)
		return -1;

	switch(operand[0]){
		case 'A':
			return A;
			break;
		case 'B':
			return reg_B;
			break;
		case 'T':
			return T;
			break;
		case 'S':
			return S;
			break;
		case 'L':
			return L;
			break;
		case 'X':
			return reg_X;
			break;
		default:
			return -1;
			break;
	}
}

/*
 * �Ľ̵� ��ū �ϳ�(instruction)�� size�� ����
 */
int get_inst_size(token* token){
	int i, index;
	char* operator = token->operator;
	char* start = 0;

	if(operator == NULL)
		return -1;

	if(operator[0] == '+')
		index = search_opcode(&operator[1]);
	else
		index = search_opcode(operator);

	if(index != -1){
		if(strstr(inst[index][1], "/") != NULL){
			//���Ŀ� / ���ڰ� ���� => 3 �Ǵ� 4
			if(operator[0] == '+')
				return 4;
			else
				return 3;
		}else
			return atoi(inst[index][1]);
	}else{
		if((strcmp(operator, "START") && strcmp(operator, "CSECT")) == 0){
			return 0;

		}else if((strcmp(operator, "END") && strcmp(operator, "LTORG") && strcmp(operator, "EXTDEF") && strcmp(
				operator, "EXTREF")) == 0){
			return -1;

		}else if(strcmp(operator, "RESB") == 0){
			return atoi(token->operand[0]);

		}else if(strcmp(operator, "RESW") == 0){
			return atoi(token->operand[0]) * 3;

		}else if(strcmp(operator, "BYTE") == 0){
			return get_byte_size(token->operand[0]);

		}else if(strcmp(operator, "WORD") == 0){
			return 3;
		}else{
			return -1;
		}
	}

	return -1;
}

/*
 * BYTE operand�� ũ�� ����
 */
int get_byte_size(char* operand){
	int i;
	char* start;

	switch(operand[0]){
		case 'X':
			start = strstr(operand, "'") + 1;
			for(i = 0; i < strlen(start); i++)
				if(start[i] == '\'')
					return i / 2 + i % 2;
			break;
		case 'C':
			start = strstr(operand, "'") + 1;
			for(i = 0; i < strlen(start); i++)
				if(start[i] == '\'')
					return i;
			break;
		default:
			i = strlen(dectohex(atoi(operand[0])));
			return i / 2 + i % 2;
			break;
	}
}

int is_digit(char* str){
	int i;

	for(i = 0; i < strlen(str); i++)
		if(!(str[i] >= '0' && str[i] <= '9'))
			return 0;

	return 1;
}
