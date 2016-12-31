/*
 * myenv.c
 *
 *  Created on: 2015. 11. 3.
 *      Author: panda
 */
#include <stdlib.h>
//#include <stdio.h>

void myenv(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("myenv");
		return;
	}
	if(arg == 0 || strlen(arg) == 0){
		extern char** environ;
		int i = 1;
		char *s = *environ;

		for (; s; i++){
			printf("%s\n", s);
			s = *(environ+i);
		}
	}else{
		printf("%s\n", getenv(arg));
	}
}
