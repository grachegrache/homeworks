/*
 * main.c
 *
 *  Created on: 2015. 5. 20.
 *      Author: panda
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define BUF_SIZE 65000
void error_handling(char *message);
char* system_func(char* operator);

int main(int argc, char *argv[]){
	//변수 초기화
	int i, sock, str_len, host_index;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	char buf[BUF_SIZE];
	char* tmp;
	char* addr;
	char* port = "10000";
	ssize_t ret;
	memset(message,0,BUF_SIZE);
	memset(buf,0,BUF_SIZE);

	if(argc != 2){
		printf("Input File Path\n");
		return -1;
	}
	
	//파일로부터 message 구하기
	int op_len = strlen(argv[1])+10;
	char op[op_len];
	memset(op,0,op_len);
	strcpy(op,"cat ");
	strcat(op, argv[1]);
	strcpy(message, system_func(op));
	message[strlen(message)-1] = 0;
	
	//addr 구하기
	tmp = strstr(message, "Host:");
	if(tmp == NULL){
		printf("Invalid Http Message");
		return -1;
	}
	tmp += 5;
	for(i=0;i<strlen(tmp);i++)
		if(tmp[i] == '\n')
			break;
	addr = strndup(tmp,i);
	
	//클라이언트 소켓 만들기
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(addr);
	serv_addr.sin_port = htons(atoi(port));

	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
		error_handling("connect() error!");
		return -1;
	}

	str_len = write(sock, message, sizeof(message));
	if(str_len == -1){
		error_handling("write() error!");
		return -1;
	}

	//EOF까지 읽기
	char* ptr_buf = buf;
	while((ret = read(sock, ptr_buf, 10)) != 0){
		if(ret == -1){
			if(errno == EINTR)
				continue;
			perror("read");
			break;
		}
		ptr_buf += ret;
	}

	if(ret == 0)
		ptr_buf[0] = 0;

	printf("%s", buf);
	close(sock);
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
		return -1;
	}

	// read the result
	readSize = fread((void*) pszBuff, sizeof(char), BUF_SIZE - 1, fp);

	if(readSize == 0){
		pclose(fp);
		printf("error [%d:%s]\n", errno, strerror(errno));
		return -1;
	}

	pclose(fp);
	pszBuff[readSize] = 0;

	return pszBuff;
}
