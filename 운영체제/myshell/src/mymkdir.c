/*
 * mymkdir.c
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#include "headers/mymkdir.h"

void mymkdir(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("mymkdir");
		return;
	}
	int fd, cnt, i, ret;
	char* absolute_path;
	char** params = (char**) malloc(sizeof(char*));
	cnt = parse_arg(arg, params);
	ret = 0;

	if(cnt < 1 || cnt > 2){
		printf("error:not enough argument!\n");
		print_man("mymkdir");
		i=0;
		while(params[i] != 0)
			free(params[i++]);
		free(params);
		return;
	}

	if(cnt == 2){
		if(strcmp(params[0], "-p") != 0){
			printf("error:unknown option!\n");
			print_man("mymkdir");
			i=0;
			while(params[i] != 0)
				free(params[i++]);
			free(params);
			return;
		}

		absolute_path = create_directories(params[1]);
		if(absolute_path == 0)
			ret = -1;
	}else{
		if((ret = mkdir(params[0], 0777)) != -1)
			absolute_path = get_absolute_path(params[0]);
	}

	if(ret != -1){
		printf("%s\n", absolute_path);
		free(absolute_path);
	}

	for(i=0;i<cnt;i++)
		free(params[i]);

	free(params);
}

char* create_directories(char* path){
	int i, j;
	char* absolute_path = get_absolute_path(path);
	char tmp[MAX_PATH];
	i = 0;

	while(i < strlen(absolute_path)){
		while(absolute_path[++i] != '/')
			if(i >= strlen(absolute_path))
				break;
		memset(tmp, 0, MAX_PATH);
		strncpy(tmp, absolute_path, i);

		if(mkdir(tmp, 0777) == -1)
			if(errno != EEXIST){
				printf("error[%d]: there are some error!\n", errno);
				free(absolute_path);
				return 0;
			}
	}

	return absolute_path;
}

char* get_absolute_path(char* path){
	char* absolute_path = (char*) calloc(MAX_PATH, sizeof(char));
	memset(absolute_path,0,MAX_PATH);

	if(strstr(path, "~/") == path){
		strcpy(absolute_path, get_homedir());
		strcpy(absolute_path, strcat(absolute_path, path+2));

	}else if(strstr(path, "/") == path){
		strcpy(absolute_path, path);

	}else if(strstr(path, "./") == path){
		strcpy(absolute_path, full_path);
		strcpy(absolute_path, strcat(absolute_path, path+2));

	}else if(strstr(path, "../") == path){
		int i=strlen(full_path);
		while(full_path[--i] != '/');
		strncpy(absolute_path, full_path, i);
		strcpy(absolute_path, strcat(absolute_path, path+3));

	}else{
		strcpy(absolute_path, full_path);
		if(full_path[strlen(full_path)-1] != '/')
			strcpy(absolute_path, strcat(absolute_path, "/"));
		strcpy(absolute_path, strcat(absolute_path, path));

	}

	return absolute_path;
}

