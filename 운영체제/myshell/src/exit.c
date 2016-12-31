/*
 * exit.c
 *
 *  Created on: 2015. 10. 27.
 *      Author: panda
 */

void myexit(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("exit");
		return;
	}

	printf("exit myshell\n");
	exit(1);
}
