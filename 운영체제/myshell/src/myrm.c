/*
 * myrm.c
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#include "headers/myrm.h"

void myrm(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("myrm");
		return;
	}
	int cnt, i;
	char** params = (char**) malloc(sizeof(char*));
	cnt = parse_arg(arg, params);
	if(cnt < 1 || cnt > 2){
		printf("error:not enough argument!\n");
		print_man("myrm");
		i = 0;
		while(params[i] != 0)
			free(params[i++]);
		return;
	}

	if(cnt == 2){
		if(strcmp(params[0], "-rf") != 0){
			printf("error:unknown option!\n");
			print_man("myrm");
			i = 0;
			while(params[i] != 0)
				free(params[i++]);
			return;
		}

		remove_directories(params[1]);

	}else{
		if(unlink(params[0]) == -1){
			if(errno == EISDIR)
				printf("error:can not remove the directory!\n");
			else
				printf("error[%d]: there are some error!\n", errno);
		}
	}

	i = 0;
	while(params[i] != 0)
		free(params[i++]);
}

void remove_directories(char* filename){
	if(remove(filename) == -1){
		if(errno == ENOTEMPTY){
			DIR* d;
			struct dirent* dir;
			char path[MAX_PATH];

			d = opendir(filename);
			if(d){
				while((dir = readdir(d)) != 0){
					if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
						memset(path, 0, MAX_PATH);
						strcpy(path, strcat(filename, "/"));
						strcpy(path, strcat(path, dir->d_name));
						if(remove(path) == -1)
							if(errno == ENOTEMPTY)
								remove_directories(path);
					}
				}
				close(d);
			}
		}
	}
	remove(filename);
}
