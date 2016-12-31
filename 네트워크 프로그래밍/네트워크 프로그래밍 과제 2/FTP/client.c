/*
 * client.c
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

#define BUF_SIZE 10000
void error_handling(char *message);
char* check_format(char* input);

int main(int argc, char *argv[]){
	//변수 초기화
	int i, sock, file_sock, str_len, host_index;
	struct sockaddr_in serv_addr, file_serv;
	char message[BUF_SIZE];
	char buf[BUF_SIZE];
	char* tmp;
	char* addr = "127.0.0.1";
	char* port1 = "10001";
	char* port2;
	ssize_t ret;
	memset(message, 0, BUF_SIZE);
	memset(buf, 0, BUF_SIZE);

	if(argc == 2)
		addr = argv[1];

	//클라이언트 소켓 만들기
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(addr);
	serv_addr.sin_port = htons(atoi(port1));

	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
		error_handling("connect() error!");
		return -1;
	}

	//passive mode 파일 입출력을 위한 소켓 포트 번호 받기
	str_len = read(sock, buf, sizeof(buf));
	if(str_len > 0)
		buf[str_len] = 0;
	else{
		error_handling("read() error!");
		return -1;
	}

	port2 = strdup(buf + 3);
	memset(buf, 0, BUF_SIZE);

	//명령
	while(1){
		if(scanf("%[^\n]s", message) > 0){
			getchar();
			if((tmp = check_format(message)) != NULL){

				//서버에 message 전송
				str_len = write(sock, message, sizeof(message));
				if(str_len == -1){
					error_handling("write() error!");
					return -1;
				}

				//message에 따라 클라이언트에서 할 일
				if(strcmp(tmp, "QUIT") == 0)
					break;

				else if(strcmp(tmp, "STOR") == 0){
					//파일 전송
					FILE* fp = fopen(message + 5, "r");
					if(fp == NULL)
						continue;
					fseek(fp, 0, SEEK_END);
					int file_len = ftell(fp);
					rewind(fp);
					char str_file[10];
					memset(str_file, 0, 10);
					sprintf(str_file,"%d", file_len);
					
					//파일 입출력을 위한 소켓 만들기
					file_sock = socket(PF_INET, SOCK_STREAM, 0);
					if(file_sock == -1)
						error_handling("socket() error");
					memset(&file_serv, 0, sizeof(file_serv));
					file_serv.sin_family = AF_INET;
					file_serv.sin_addr.s_addr = inet_addr(addr);
					file_serv.sin_port = htons(atoi(port2));

					printf("connecting...\n");
					sleep(5);
					if(connect(file_sock, (struct sockaddr*) &file_serv, sizeof(file_serv))){
						error_handling("connect() error");
						return -1;
					}

					write(file_sock, str_file, 10);

					printf("writing...\n");
					memset(message, 0, BUF_SIZE);
					while(fgets(message, BUF_SIZE, fp) != NULL){
						if(write(file_sock, message, sizeof(message)) == -1){
							error_handling("write() error!");
							return -1;
						}
						memset(message, 0, BUF_SIZE);
					}

					printf("complete\n");

					fclose(fp);
					close(file_sock);
					
				}else if(strcmp(tmp, "RETR") == 0){
					//파일 이름 구하기
					int i;
					for(i = strlen(message) - 1; i > 0; i--)
						if(message[i] == '/'){
							i++;
							break;
						}

					//파일 열기
					FILE* fp = fopen(message + i, "w+");

					//파일 입출력을 위한 소켓 만들기
					file_sock = socket(PF_INET, SOCK_STREAM, 0);
					if(file_sock == -1)
						error_handling("socket() error");
					memset(&file_serv, 0, sizeof(file_serv));
					file_serv.sin_family = AF_INET;
					file_serv.sin_addr.s_addr = inet_addr(addr);
					file_serv.sin_port = htons(atoi(port2));

					//파일 입출력을 위한 소켓 연결
					printf("connecting...\n");
					sleep(5);
					if(connect(file_sock, (struct sockaddr*) &file_serv, sizeof(file_serv))){
						error_handling("connect() error");
						return -1;
					}

					//파일 크기 받기
					ret = read(file_sock, buf, 10);
					if(ret > 0)
						buf[ret] = 0;
					if(strcmp(buf, "ERROR") == 0)
						return -1;
					
					int file_size = atoi(buf);
					int recv_len = 0;
					memset(buf, 0, BUF_SIZE);
					
					printf("saving...\n");

					//파일 저장
					while(file_size > recv_len){
						ret = read(file_sock, buf, BUF_SIZE);
						if(ret > 0){
							recv_len += ret;
							fputs(buf, fp);
							memset(buf, 0, BUF_SIZE);
						}
					}

					printf("complete\n");

					fclose(fp);
					close(file_sock);
				}

				ret = read(sock, buf, BUF_SIZE);
				if(ret > 0)
					buf[ret] = 0;

				printf("%s\n", buf);
				memset(buf, 0, BUF_SIZE);
				memset(message, 0, BUF_SIZE);
			}

		}
	}

	free(port2);
	close(sock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

char* check_format(char* input){
	int i, op_type;
	op_type = 0;
	char* op;

	if(strstr(input, "PWD") != NULL){
		op = "PWD";
		op_type = 1;
	}else if(strstr(input, "CWD") != NULL){
		op = "CWD";
		op_type = 2;
	}else if(strstr(input, "LIST") != NULL){
		op = "LIST";
		op_type = 1;
	}else if(strstr(input, "NLST") != NULL){
		op = "NLST";
		op_type = 1;
	}else if(strstr(input, "STOR") != NULL){
		op = "STOR";
		op_type = 2;
	}else if(strstr(input, "RETR") != NULL){
		op = "RETR";
		op_type = 2;
	}else if(strstr(input, "DELE") != NULL){
		op = "DELE";
		op_type = 2;
	}else if(strstr(input, "MKD") != NULL){
		op = "MKD";
		op_type = 2;
	}else if(strstr(input, "RMD") != NULL){
		op = "RMD";
		op_type = 2;
	}else if(strstr(input, "QUIT") != NULL){
		op = "QUIT";
		op_type = 1;
	}

	switch(op_type){
		case 1:
			if(strcmp(input, op) == 0)
				return op;
			else
				return NULL;
		case 2:
			for(i = 0; i < strlen(input); i++)
				if(input[i] == ' '){
					i++;
					break;
				}
			if(input[i] == '/')
				return op;
			else
				return NULL;
		default:
			return NULL;
	}
}
