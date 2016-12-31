/*
 * server.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: panda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#define BUF_SIZE 10000

typedef struct {
	char* op;
	char* path;
	char* file;
} OP;

void error_handling(char *message);
char* system_func(char* operator);
OP* parse_op(char* buf);
char* parse_list(char* list);
char* get_homedirectory();
char* replace_all(char* s, const char* olds, const char* news);
char* make_message(OP* op);

char* cwd;
int serv_sock, clnt_sock, file_serv_sock, file_clnt_sock, optlen, option;
struct sockaddr_in serv_addr, clnt_addr, file_serv, file_clnt;
socklen_t clnt_addr_size;

int main(int argc, char *argv[]){
	//변수 초기화
	char buf[BUF_SIZE];
	char message[BUF_SIZE];
	char* port1 = "10001";
	char* port2 = "10002";
	ssize_t ret;
	memset(message, 0, sizeof(message));

	//home directory 경로 얻기
	cwd = get_homedirectory();

	//서버 소켓 만들기
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port1));

	file_serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(file_serv_sock == -1)
		error_handling("socket() error");
	memset(&file_serv, 0, sizeof(file_serv));
	file_serv.sin_family = AF_INET;
	file_serv.sin_addr.s_addr = htonl(INADDR_ANY);
	file_serv.sin_port = htons(atoi(port2));

	//set REUSEADDR
	optlen = sizeof(option);
	option = true;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*) &option, optlen);
	setsockopt(file_serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*) &option, optlen);

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
		error_handling("bind() error");
		return -1;
	}

	if(bind(file_serv_sock, (struct sockaddr*) &file_serv, sizeof(file_serv)) == -1){
		error_handling("bind() error");
		return -1;
	}

	if(listen(serv_sock, 5) == -1){
		error_handling("listen() error");
		return -1;
	}

	if(listen(file_serv_sock, 5) == -1){
		error_handling("listen() error");
		return -1;
	}
	
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1){
		error_handling("accept() error");
		return -1;
	}

	//클라이언트에게 포트 번호 전송
	strcpy(message, "OK ");
	strcat(message, port2);
	write(clnt_sock, message, sizeof(message));
	memset(message, 0, BUF_SIZE);

	while(true){
//		if(listen(serv_sock, 5) == -1){
//			error_handling("listen() error");
//			return -1;
//		}
//
//		clnt_addr_size = sizeof(clnt_addr);
//		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
//		if(clnt_sock == -1){
//			error_handling("accept() error");
//			return -1;
//		}

		ret = read(clnt_sock, buf, BUF_SIZE);
		if(ret > 0)
			buf[ret] = 0;

		//명령어 실행 및 메시지 만들기0
		char* msg = make_message(parse_op(buf));
		if(msg == NULL)
			break;

		strcpy(message, msg);

		//write
		write(clnt_sock, message, sizeof(message));

	}

	close(clnt_sock);
	close(file_clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

/*
 * 리눅스 터미널 명령어를 실행하고 결과를 리턴하는 함수
 */
char* system_func(char* operator){
	FILE *fp = NULL;
	size_t readSize = 0;
	char pszBuff[BUF_SIZE];

	// excute command
	fp = popen(operator, "r");
	if(!fp){
		printf("error [%d:%s]\n", errno, strerror(errno));
		return "ERROR";
	}

	// read the result
	readSize = fread((void*) pszBuff, sizeof(char), BUF_SIZE - 1, fp);

	if(readSize == 0){
		pclose(fp);
		printf("error [%d:%s]\n", errno, strerror(errno));
		return "ERROR";
	}

	pclose(fp);
	pszBuff[readSize] = 0;

	return pszBuff;
}

OP* parse_op(char* buf){
	OP* op = malloc(sizeof(OP));

	//op, path 파싱
	if(strstr(buf, " ") != NULL){
		op->op = strtok(buf, " ");
		op->path = strtok(NULL, "\n");
	}else{
		op->op = buf;
		op->path = NULL;
	}

	//file 파싱
	if(op->path != NULL){
		int i;
		for(i = strlen(op->path) - 1; i > 0; i--)
			if(op->path[i] == '/'){
				i++;
				break;
			}
		op->file = strdup(op->path + i);
	}else
		op->file = NULL;

	return op;
}

char* parse_list(char* list){
	if(list == NULL)
		return "ERROR";
	int i;
	char* cpy = strdup(list);
	char* tok;
	char str[BUF_SIZE];
	memset(str, 0, BUF_SIZE);

	tok = strtok(cpy, "\n");

	if(tok != NULL){
		for(i = strlen(tok) - 1; i > 0; i--)
			if(tok[i] == ' ' && tok[i - 1] >= '0' && tok[i - 1] <= '9'){
				i++;
				break;
			}
		strcpy(str, strdup(tok + i));

		while((tok = strtok(NULL, "\n")) != NULL){
			for(i = strlen(tok) - 1; i > 0; i--)
				if(tok[i] == ' ' && tok[i - 1] >= '0' && tok[i - 1] <= '9')
					break;

			strcat(str, strdup(tok + i));
		}

		free(cpy);
		return str;
	}else
		return NULL;
}

//home directory 경로 얻기
char* get_homedirectory(){
	struct passwd* user_pw;
	user_pw = getpwuid(getuid());
	return user_pw->pw_dir;
}

char* replace_all(char* s, const char* olds, const char* news){
	char *result, *sr;
	size_t i, count = 0;
	size_t oldlen = strlen(olds);
	if(oldlen < 1)
		return s;
	size_t newlen = strlen(news);

	if(newlen != oldlen){
		for(i = 0; s[i] != '\0';){
			if(memcmp(&s[i], olds, oldlen) == 0)
				count++, i += oldlen;
			else
				i++;
		}
	}else
		i = strlen(s);

	result = (char *) malloc(i + 1 + count * (newlen - oldlen));
	if(result == NULL)
		return NULL;

	sr = result;
	while(*s){
		if(memcmp(s, olds, oldlen) == 0){
			memcpy(sr, news, newlen);
			sr += newlen;
			s += oldlen;
		}else
			*sr++ = *s++;
	}
	*sr = '\0';

	return result;
}

char* make_message(OP* op){
	int ret;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);
	char* path;

	//path 만들기
	if(op->path != NULL){
		int path_len = strlen(cwd) + strlen(op->path);
		path = calloc(path_len, sizeof(char));
		memset(path, 0, path_len);
		strcpy(path, cwd);
		strcat(path, op->path);
	}else
		path = cwd;

	if(strcmp(op->op, "PWD") == 0){
		char* replaced = replace_all(cwd, get_homedirectory(), "");
		if(replaced != NULL && strlen(replaced) == 0)
			return "/";
		else
			return replaced;

	}else if(strcmp(op->op, "CWD") == 0){
		char* home = get_homedirectory();
		int cwd_len = strlen(home) + strlen(op->path);
		cwd = calloc(cwd_len, sizeof(char));
		memset(cwd, 0, cwd_len);

		strcpy(cwd, home);
		if(strlen(op->path) > 1)
			strcat(cwd, op->path);

		return "OK";

	}else if(strcmp(op->op, "LIST") == 0){
		char* operation = calloc(strlen(cwd) + 20, sizeof(char));
		memset(operation, 0, strlen(cwd) + 20);
		strcpy(operation, "ls -l ");
		strcat(operation, cwd);
		strcat(operation, " | grep \"^-\"");
		char* list = system_func(operation);
		free(operation);
		return parse_list(list);

	}else if(strcmp(op->op, "NLST") == 0){
		char* operation = calloc(strlen(cwd) + 20, sizeof(char));
		memset(operation, 0, strlen(cwd) + 20);
		strcpy(operation, "ls -l ");
		strcat(operation, cwd);
		strcat(operation, " | grep \"^d\"");
		char* list = system_func(operation);
		free(operation);
		return parse_list(list);

	}else if(strcmp(op->op, "STOR") == 0){
		//클라이언트 연결

		clnt_addr_size = sizeof(file_clnt);
		file_clnt_sock = accept(file_serv_sock, (struct sockaddr*) &file_clnt, &clnt_addr_size);
		if(file_clnt_sock == -1){
			error_handling("accept() error");
			return "ERROR";
		}

		char* file;
		FILE* fp = NULL;

		//복사할 파일 경로 만들기
		int file_len = strlen(cwd) + strlen(op->file) + 1;
		file = calloc(file_len, sizeof(char));
		memset(file, 0, file_len);
		strcpy(file, cwd);
		strcat(file, "/");
		strcat(file, op->file);

		//파일 크기 받기
		ret = read(file_clnt_sock, buf, 10);
		if(ret > 0)
			buf[ret] = 0;
		int file_size = atoi(buf);
		int recv_len = 0;
		memset(buf, 0, BUF_SIZE);

		//파일 열기
		if((fp = fopen(file, "w+")) == NULL)
			return "ERROR";

		//파일 복사 저장
		while(file_size > recv_len){
			ret = read(file_clnt_sock, buf, BUF_SIZE);
			if(ret > 0){
				recv_len += ret;
				fputs(buf, fp);
				memset(buf, 0, BUF_SIZE);
			}
		}

		fclose(fp);

		return "OK";

	}else if(strcmp(op->op, "RETR") == 0){
		//클라이언트 연결

		clnt_addr_size = sizeof(file_clnt);
		file_clnt_sock = accept(file_serv_sock, (struct sockaddr*) &file_clnt, &clnt_addr_size);
		if(file_clnt_sock == -1){
			error_handling("accept() error");
			return "ERROR";
		}

		//파일 열기
		FILE* fp = NULL;
		if((fp = fopen(path, "r")) == NULL)
			return "ERROR";

		fseek(fp, 0, SEEK_END);
		int file_len = ftell(fp);
		rewind(fp);
		char str_file[10];
		memset(str_file, 0, 10);
		sprintf(str_file, "%d", file_len);

		//파일 크기 전송
		write(file_clnt_sock, str_file, 10);

		//파일 전송
		while(fgets(buf, BUF_SIZE, fp) != NULL){
			if(write(file_clnt_sock, buf, sizeof(buf)) == -1){
				error_handling("write() error!");
				return "ERROR";
			}
			memset(buf, 0, BUF_SIZE);
		}

		fclose(fp);

		return "OK";

	}else if(strcmp(op->op, "DELE") == 0){
		char* operation = calloc(strlen(path) + 3, sizeof(char));
		memset(operation, 0, strlen(path) + 3);
		strcpy(operation, "rm ");
		strcat(operation, path);
		system(operation);
		free(operation);
		return "OK";

	}else if(strcmp(op->op, "MKD") == 0){
		char* operation = calloc(strlen(path) + 6, sizeof(char));
		memset(operation, 0, strlen(path) + 6);
		strcpy(operation, "mkdir ");
		strcat(operation, path);
		system(operation);
		free(operation);
		return "OK";

	}else if(strcmp(op->op, "RMD") == 0){
		char* msg;
		char* operation = calloc(strlen(path) + 6, sizeof(char));
		memset(operation, 0, strlen(path) + 6);
		strcpy(operation, "rmdir ");
		strcat(operation, path);
		if(strstr(system_func(operation), "fail") == NULL)
			msg = "OK";
		else
			msg = "ERROR";
		free(operation);
		return msg;

	}else if(strcmp(op->op, "QUIT") == 0){
		return NULL;
	}else
		return "ERROR";
}
