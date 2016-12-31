/*
 * myrmdir.c
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#include <errno.h>

void myrmdir(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("myrmdir");
		return;
	}
	if(rmdir(arg) == -1){
		if(errno == ENOTDIR)
			printf("error:can not remove the file!\n");
		else if(errno == ENOTEMPTY)
			printf("error:not an empty directory!\n");
		else
			printf("error[%d]: there are some error!\n", errno);
	}
}
