/*
 * mycat.c
 *
 *  Created on: 2015. 11. 7.
 *      Author: panda
 */

#include "headers/about_file.h"

void mycat(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("mycat");
		return;
	}
	int i, param_count, number, fd, count, read_count;
	char buf[1];
	char** params = (char**) malloc(sizeof(char*));
	param_count = parse_arg(arg, params);
	count = 0;
	read_count = 0;

	if(param_count < 1 || param_count > 2){
		printf("error:not enough argument!\n");
		print_man("mycat");
		free(params);
		return;
	}

	if(param_count == 2)
		number = atoi(params[1]);
	else
		number = -1;

	fd = open(params[0], O_RDONLY);
	if(fd == -1){
		printf("error:can not open the file!\n");
		print_man("mycat");
		return;
	}

	if(number == -1)
		while(read(fd, buf, 1) > 0)
			printf("%s", buf);
	else
		while((count = read(fd, buf, 1)) > 0){
			printf("%s", buf);
			read_count += count;
			if(read_count >= number)
				break;
		}

	close(fd);
	for(i=0;i<param_count;i++)
		free(params[i]);
	free(params);
}
