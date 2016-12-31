/*
 * mypwd.c
 *
 *  Created on: 2015. 11. 3.
 *      Author: panda
 */

extern char full_path[];

void mypwd(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("mypwd");
		return;
	}
	printf("%s\n", full_path);
}
