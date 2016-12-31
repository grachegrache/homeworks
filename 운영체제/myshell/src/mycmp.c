/*
 * mycmp.c
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#include "headers/about_file.h"

void mycmp(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("mycmp");
		return;
	}
	int fd1, fd2, byte_point, cnt, cnt_line, i;
	char buf1[1], buf2[1];
	char** params = (char**) malloc(sizeof(char*));
	cnt_line = 1;
	cnt = parse_arg(arg, params);
	if(cnt != 2){
		printf("error:not enough argument!\n");
		print_man("mycmp");
		for(i=0;i<cnt;i++)
			free(params[i]);
		free(params);
		return;
	}

	fd1 = open(params[0], O_RDONLY);
	fd2 = open(params[1], O_RDONLY);

	if(fd1 == -1 || fd2 == -1){
		printf("error:can not open the file!\n");
		print_man("mycmp");

		if(fd1 != -1)
			close(fd1);
		if(fd2 != -1)
			close(fd2);
		return;
	}

	while(read(fd1, buf1, 1) > 0 && read(fd2, buf2, 1) > 0){
		if(buf1[0] == '\n')
			cnt_line++;

		if(buf1[0] != buf2[0])
			break;
	}

	byte_point = lseek(fd1, 0, SEEK_CUR);
	if(lseek(fd1, 0, SEEK_END) != lseek(fd2, 0, SEEK_END))
		printf("%s %s differ: byte %d, line %d\n", params[0],params[1],byte_point,cnt_line);

	for(i=0;i<cnt;i++)
		free(params[i]);
	free(params);
	close(fd1);
	close(fd2);
}
