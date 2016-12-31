/*
 * cd.c
 *
 *  Created on: 2015. 11. 3.
 *      Author: panda
 */

#include "headers/cd.h"

void mycd(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("cd");
		return;
	}
	int i, j, quotation;
	quotation = 0;

	if(strlen(arg) == 0)
		arg = "~";

	for(i=0;i<strlen(arg);i++){
		if(arg[i] == '\"'){
			if(++quotation == 3){
				print_man("cd");
				return;
			}

			//push left
			for(j=i;j<strlen(arg)-1;j++)
				arg[j] = arg[j+1];
			arg[j] = 0;
		}

		if(quotation == 0 && arg[i] == ' '){
			print_man("cd");
			return;
		}
	}

	if(arg[0] != '~' && access(arg, 0) != 0){
		printf("error:not exist file!\n");
		print_man("cd");
		return;

	}else{
		//if the arg is correct change the paths
		switch(arg[0]){
			case '~':
				strcpy(full_path, get_homedir());
				strcpy(full_path, strcat(full_path, arg+1));
				break;
			case '/':
				strcpy(full_path, arg);
				break;
			case '.':
				if(arg[1] == '.'){
					for(i=strlen(full_path);full_path[i] != '/';i--);
					full_path[i] = 0;
					strcpy(full_path, strcat(full_path, arg+2));
				}else
					strcpy(full_path, strcat(full_path, arg+1));
				break;
			default:
				strcpy(full_path, strcat(full_path, "/"));
				strcpy(full_path, strcat(full_path, arg));
				break;
		}
		set_path();
		chdir(full_path);
	}
}

void set_path(){
	int i;
	for(i=strlen(full_path);full_path[i] != '/';i--);
	strncpy(path, full_path+i+1,strlen(full_path)-i);
}

char* get_homedir(){
	char* homedir;

	if((homedir = getenv("HOME")) == NULL)
	    homedir = getpwuid(getuid())->pw_dir;

	return homedir;
}
