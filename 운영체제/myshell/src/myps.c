/*
 * myps.c
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#include "headers/myps.h"

void myps(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("myps");
		return;
	}
	char path[MAX_PATH];
	DIR* d;
	struct dirent* dir;
	memset(path, 0, MAX_PATH);

	printf("<UID>\t<PID>\t<PPID>\t<STIME>\t\t<CMD>\n");

	d = opendir("/proc");
	if(d){
		while((dir = readdir(d)) != 0){
			if(is_digit(dir->d_name)){
				//if the directory name is number
				sprintf(path, "/proc/%s/stat", dir->d_name);
				if(access(path, F_OK) == 0)
					//if the file does exist
					print_stat(path);
			}
		}

		close(d);
	}

}

int is_digit(char* str){
	int i;
	for(i=0;i<strlen(str);i++)
		if(isdigit(str[i]) == 0)
			return 0;
	return 1;
}

void print_stat(char* stat_path){
	int fd, cnt;
	char buf[1];
	struct stat s;
	string* pid;
	string* ppid;
	string* stime;
	string* cmd;
	string* tmp_pid;
	string* tmp_ppid;
	string* tmp_stime;
	string* tmp_cmd;
	cnt = 0;

	pid = (string*) malloc(sizeof(string));
	ppid = (string*) malloc(sizeof(string));
	stime = (string*) malloc(sizeof(string));
	cmd = (string*) malloc(sizeof(string));
	pid->next = 0;
	ppid->next = 0;
	stime->next = 0;
	cmd->next = 0;
	tmp_pid = pid;
	tmp_ppid = ppid;
	tmp_stime = stime;
	tmp_cmd = cmd;

	fd = open(stat_path, O_RDONLY);
	while(read(fd, buf, 1) > 0){
		if(buf[0] == ' '){
			switch(cnt){
				case 0:
					//pid
					if(tmp_pid != 0)
						tmp_pid->ch = 0;
					break;
				case 1:
					//cmd
					if(tmp_cmd != 0)
						tmp_cmd->ch = 0;
					break;
				case 3:
					//ppid
					if(tmp_ppid != 0)
						tmp_ppid->ch = 0;
					break;
				case 21:
					//stime
					if(tmp_stime != 0)
						tmp_stime->ch = 0;
					break;
			}
			cnt++;

		}else{
			switch(cnt){
				case 0:
					//pid
					tmp_pid->ch = buf[0];
					tmp_pid->next = (string*) malloc(sizeof(string));
					tmp_pid = tmp_pid->next;
					tmp_pid->next = 0;
					break;
				case 1:
					//cmd
					if(buf[0] != '(' && buf[0] != ')'){
						tmp_cmd->ch = buf[0];
						tmp_cmd->next = (string*) malloc(sizeof(string));
						tmp_cmd = tmp_cmd->next;
						tmp_cmd->next = 0;
					}
					break;
				case 2:
					//check running state
					if(buf[0] != 'R'){
						free_string(pid);
						free_string(cmd);
						close(fd);
						return;
					}
					break;
				case 3:
					//ppid
					tmp_ppid->ch = buf[0];
					tmp_ppid->next = (string*) malloc(sizeof(string));
					tmp_ppid = tmp_ppid->next;
					tmp_ppid->next = 0;
					break;
				case 21:
					//stime
					tmp_stime->ch = buf[0];
					tmp_stime->next = (string*) malloc(sizeof(string));
					tmp_stime = tmp_stime->next;
					tmp_stime->next = 0;
					break;
			}
		}

		if(cnt == 22)
			break;
	}
	close(fd);

	tmp_pid = pid;
	tmp_ppid = ppid;
	tmp_stime = stime;
	tmp_cmd = cmd;

	stat(stat_path, &s);
	printf("%d\t", s.st_uid);
	while(tmp_pid != 0){
		if(tmp_pid->ch != 0)
			printf("%c", tmp_pid->ch);
		tmp_pid = tmp_pid->next;
	}
	printf("\t");
	while(tmp_ppid != 0){
		if(tmp_ppid->ch != 0)
			printf("%c", tmp_ppid->ch);
		tmp_ppid = tmp_ppid->next;
	}
	printf("\t");
	while(tmp_stime != 0){
		if(tmp_stime->ch != 0)
			printf("%c", tmp_stime->ch);
		tmp_stime = tmp_stime->next;
	}
	printf("\t\t");
	while(tmp_cmd != 0){
		if(tmp_cmd->ch != 0)
			printf("%c", tmp_cmd->ch);
		tmp_cmd = tmp_cmd->next;
	}
	printf("\n");

	free_string(pid);
	free_string(ppid);
	free_string(stime);
	free_string(cmd);
}

void free_string(string* str){
	if(str->next != 0)
		free_string(str->next);
	if(str != 0)
		free(str);
}
