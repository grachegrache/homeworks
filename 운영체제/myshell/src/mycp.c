/*
 * mycp.c
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#include "headers/about_file.h"

void mycp(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("mycp");
		return;
	}
	int fd_src, fd_dst, cnt, i;
	char buf[1];
	char** params = (char**) malloc(sizeof(char*));
	cnt = parse_arg(arg, params);
	if(cnt < 2 || cnt > 3){
		printf("error:not enough argument!\n");
		print_man("mycp");
		i = 0;
		while(params[i] != 0)
			free(params[i++]);
		free(params);
		return;
	}

	if(cnt == 3){
		if(strcmp(params[0], "-f") != 0){
			printf("error:unknown option!\n");
			print_man("mycp");
			i = 0;
			while(params[i] != 0)
				free(params[i++]);
			free(params);
			return;
		}

		fd_src = open(params[1], O_RDONLY);
		fd_dst = open(params[2], O_CREAT | O_WRONLY | O_TRUNC);

	}else{
		fd_src = open(params[0], O_RDONLY);
		fd_dst = open(params[1], O_CREAT | O_WRONLY | O_EXCL);
	}

	if(fd_src == -1 || fd_dst == -1){
		printf("error:can not open the file!\n");
		print_man("mycp");
		if(fd_src != -1)
			close(fd_src);
		if(fd_dst != -1)
			close(fd_dst);

		i = 0;
		while(params[i] != 0)
			free(params[i++]);
		free(params);
		return;
	}

	while(read(fd_src, buf, 1) > 0)
		write(fd_dst, buf, 1);

	close(fd_src);
	close(fd_dst);

	if(cnt == 3)
		chmod(params[2], 0644);
	else
		chmod(params[1], 0644);

	for(i=0;i<cnt;i++)
		free(params[i]);
	free(params);
}
