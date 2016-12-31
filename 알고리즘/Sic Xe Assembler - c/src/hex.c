/*
 * hex.c
 *
 *  Created on: 2015. 5. 3.
 *      Author: panda
 */
#include "hex.h"

char get_hex(int r){
	switch(r){
		case 10:
			return 'A';
		case 11:
			return 'B';
		case 12:
			return 'C';
		case 13:
			return 'D';
		case 14:
			return 'E';
		case 15:
			return 'F';
		default:
			return r + '0';
	}
}

int get_dec(char h){
	switch(h){
		case 'A':
			return 10;
		case 'B':
			return 11;
		case 'C':
			return 12;
		case 'D':
			return 13;
		case 'E':
			return 14;
		case 'F':
			return 15;
		default:
			return h - '0';

	}
}

char* dectohex(int n){
	char result[7];
	char tmp;
	int index = 0;
	int q, r, i, rindex, minus;

	if(n == 0)
		return "0";

	else if(n < 0){
		minus = 1;
		n = -n;
	}else
		minus = 0;

	do{
		q = n / 16;
		r = n % 16;
		result[index++] = get_hex(r);
		n = q;
	}while(q >= 16);

	if(q != 0)
		result[index++] = get_hex(q);
	result[index] = 0;

//reverse
	for(i = 0; i < strlen(result) / 2; i++){
		tmp = result[i];
		rindex = strlen(result) - 1 - i;
		result[i] = result[rindex];
		result[rindex] = tmp;
	}

	return minus ? get_complement_of_hex(result, 6) : result;
}

char* get_complement_of_hex(char* hex, int size){
	int i, carry, blank;
	char* result = calloc(size, sizeof(char));
	blank = size - strlen(hex);
	result[0] = 0;

//음수로 변환
	for(i = strlen(hex) - 1; i >= 0; i--){
		if(i == strlen(hex) - 1){
			//마지막 인덱스이면
			if(hex[i] == '0')
				carry = 1;
			else{
				carry = 0;
				hex[i] = get_hex(get_dec('F') - get_dec(hex[i]) + 1);
			}
		}else{
			//마지막 인덱스가 아니면
			if(carry)
				hex[i]++;

			if('F' - hex[i] >= 0){
				hex[i] = get_hex(get_dec('F') - get_dec(hex[i]));
				carry = 0;
			}else{
				//원래 값이 'F'이고 carry값이 1일 때
				carry = 1;
				hex[i] = '1';
			}

		}
	}

	if(carry)
		return NULL;
	else{
		//앞에 size를 맞추기 위해 F 추가
		if(blank > 0){
			for(i = 0; i < blank; i++)
				strcat(result, "F");
			strcat(result, hex);
		}else{
			strcat(result, hex - blank);
		}

		return result;
	}
}

char* get_formated_hex(char* hex, int size){
	int i;
	int blank;
	int str_len;
	char* copy = hex;
	hex = calloc(size + 1, sizeof(char));
	str_len = strlen(copy);
	blank = size - str_len;
	hex[0] = 0;

	if(blank > 0){
		for(i = 0; i < blank; i++)
			strcat(hex, "0");
		strcat(hex, copy);
	}else{
		strcat(hex, copy - blank);
	}

	return hex;
}



