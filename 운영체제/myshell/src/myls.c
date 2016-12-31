/*
 * myls.c
 *
 *  Created on: 2015. 11. 3.
 *      Author: panda
 */

#include "headers/myls.h"
#define MAX_PATH 1000

void myls(char* arg) {
	if(strcmp(arg, "--help") == 0){
		print_man("myls");
		return;
	}
	DIR *d;
	struct dirent* dir;

	if(arg == 0 || strlen(arg) == 0)
		arg = ".";

	d = opendir(arg);
	if(d){
		//folder
		while((dir = readdir(d)) != 0)
			print_file(dir);

	}else{
		//check this file
		bool is_exist = false;
		d = opendir(".");
		if(d){
			while((dir = readdir(d)) != 0)
				if(strcmp(arg, dir->d_name) == 0){
					is_exist = true;
					print_file(dir);
				}

			if(is_exist == false){
				printf("error:not exist file\n");
				print_man("myls");
				return;
			}
		}
	}

	if(d)
		closedir(d);
}

char get_dir_type(unsigned char d_type){
	switch(d_type){
		case DT_DIR:
			return 'd';
		case DT_REG:
			return '-';
		case DT_BLK:
			return 'b';
		case DT_CHR:
			return 'c';
		case DT_FIFO:
			return 'f';
		case DT_SOCK:
			return 's';
		case DT_LNK:
			return 'l';
		default:
			return ' ';
	}
}

void get_stat(char* name, struct stat* s){
	int i;
	char path[MAX_PATH];
	memset(path,0,MAX_PATH);
	strcpy(path, full_path);

	if(strcmp(name, "..") == 0){
		for(i=strlen(path);path[i] != '/';i--);
		path[i] = 0;

	}else if(strcmp(name, ".") != 0){
		strcpy(path, strcat(path, "/"));
		strcpy(path, strcat(path, name));
	}

	stat(path, s);
}

void print_file(struct dirent* dir){
	struct stat s;
	get_stat(dir->d_name, &s);

	printf("%c", get_dir_type(dir->d_type));
	printf("%c", (s.st_mode & S_IRUSR) ? 'r' : '-');
	printf("%c", (s.st_mode & S_IWUSR) ? 'w' : '-');
	printf("%c", (s.st_mode & S_IXUSR) ? 'x' : '-');
	printf("%c", (s.st_mode & S_IRGRP) ? 'r' : '-');
	printf("%c", (s.st_mode & S_IWGRP) ? 'w' : '-');
	printf("%c", (s.st_mode & S_IXGRP) ? 'x' : '-');
	printf("%c", (s.st_mode & S_IROTH) ? 'r' : '-');
	printf("%c", (s.st_mode & S_IWOTH) ? 'w' : '-');
	printf("%c", (s.st_mode & S_IXOTH) ? 'x' : '-');
	printf(" %s\n", dir->d_name);

}
