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
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#define BUF_SIZE 65000

//HTTP 메시지 구조체
typedef struct {
	char* method;
	char* operand;
	char* host;
	char* body;
} http_message;

//ls 목록 구조체
typedef struct {
	char* permission;
	char* link;
	char* owner;
	char* group;
	char* size;
	char* time;
	char* name;
} ls_token;

ls_token* token_table[1000];
static int token_index = 0;

void error_handling(char *message);
char* system_func(char* operator);
http_message* get_http_method(char* buf);
char* run_http_method(http_message* msg);
char* make_http_message(http_message* msg, struct stat* st, char* input);
void parse_ls(char* ls);
char* to_json();

int main(int argc, char *argv[]){
	//변수 초기화
	int serv_sock, clnt_sock, optlen, option;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	char buf[BUF_SIZE];
	char message[BUF_SIZE];
	char* port = "10000";
	ssize_t ret;
	memset(message, 0, sizeof(message));

	//서버 소켓 만들기
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(port));

	//set REUSEADDR
	optlen = sizeof(option);
	option = true;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*) &option, optlen);

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
		error_handling("bind() error");
		return -1;
	}

	while(true){
		if(listen(serv_sock, 5) == -1){
			error_handling("listen() error");
			return -1;
		}

		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if(clnt_sock == -1){
			error_handling("accept() error");
			return -1;
		}

		ret = read(clnt_sock, buf, BUF_SIZE);
		if(ret > 0)
			buf[ret] = 0;

		// http_message 파싱
		http_message* msg = get_http_method(buf);

		// http message 실행
		strcpy(message, run_http_method(msg));

		//write
		write(clnt_sock, message, sizeof(message));
		close(clnt_sock);
	}
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

/*
 * 문자열을 HTTP 메시지 구조체로 파싱하는 함수
 */
http_message* get_http_method(char* buf){
	//변수 초기화
	int i, prev_i, count, line;
	http_message* msg = (http_message*) malloc(sizeof(http_message));
	char* tmp;
	msg->method = 0;
	msg->operand = 0;
	msg->host = 0;
	msg->body = 0;
	i = 0;
	prev_i = 0;
	count = 0;
	line = 0;
	
	for(; i < strlen(buf); i++){
		if(count == 1 && buf[prev_i] == '\"'){
			//쌍따옴표가 나오면 빈칸은 무시하고
			//다음 쌍따옴표가 나올 때까지 진행
			if(prev_i < i && buf[i] == '\"'){
				tmp = strndup(buf + prev_i + 1, i - prev_i - 1);
				msg->operand = tmp;
				i++;
				count++;
			}
		}
		
		if(buf[i] == ' ' || buf[i] == '\n' || i == strlen(buf) - 1){
			//빈칸, 개행, 마지막 문자와 만나면
			
			if(buf[i] == '\n'){
				line++;
				if(line == 3)
					prev_i++;
			}
			
			if(count > 2 && i == strlen(buf) - 1)
				i++;
			tmp = strndup(buf + prev_i, i - prev_i);
			
			switch(count){
				case 0:
					if((strcmp(tmp, "GET") && strcmp(tmp, "POST") && strcmp(tmp, "PUT") && strcmp(
							tmp, "DELETE")) == 0){
						msg->method = tmp;
						count++;
					}
					break;
				case 1:
					if(tmp[0] == '/'){
						msg->operand = tmp;
						count++;
					}
					break;
				case 2:
					if(strstr(tmp, "Host") != NULL){
						msg->host = tmp + 5;
						count++;
					}
					break;
				case 3:
					if(line > 2 && i == strlen(buf)){
						msg->body = tmp;
						count++;
					}
					break;
				default:
					break;
			}
			if(line < 3)
				prev_i = i + 1;
		}
	}

	return msg;
}

/*
 * 파싱된 HTTP 메시지로 실제 실행을 수행하는 함수
 * 클라이언트에 보낼 메시지를 리턴한다
 */
char* run_http_method(http_message* msg){
	if(msg->operand == NULL)
		return NULL;
	//변수 초기화
	char result[BUF_SIZE];
	char input[BUF_SIZE];
	memset(result, 0, BUF_SIZE);
	memset(input, 0, BUF_SIZE);

	//home directory 경로 얻기
	struct passwd* user_pw;
	user_pw = getpwuid(getuid());
	char* homedir = user_pw->pw_dir;

	//path 만들기
	int path_len = strlen(msg->operand) + strlen(homedir) + 3;
	char* path = (char*) calloc(path_len, sizeof(char));
	memset(path, 0, path_len);
//	if(strstr(msg->operand," ") != NULL)
//		strcpy(path,"\"");
	strcat(path, homedir);
	if(strlen(msg->operand) > 1)
		strcat(path, msg->operand);
//	if(strstr(msg->operand," ") != NULL)
//		strcat(path,"\"");
	printf("path: %s\n", path);

	//파일 상태 가져오기
	struct stat st;
	int fl = stat(path, &st);
	if(fl == -1)
		perror("stat");

	//명령어 초기화
	char* op = (char*) calloc(strlen(path) + 10, sizeof(char));
	memset(op, 0, strlen(path) + 10);

	if(strcmp(msg->method, "GET") == 0){
		if(fl != -1){
			if(S_ISDIR(st.st_mode)){
				//폴더이면
				strcpy(op, "ls -l ");
				strcat(op, path);
				parse_ls(system_func(op));
				strcpy(input, to_json());
			}else if(S_ISREG(st.st_mode)){
				//파일이면
				strcpy(op, "cat ");
				strcat(op, path);
				strcpy(input, system_func(op));
			}
			strcpy(result, make_http_message(msg, &st, input));
			
		}else
			strcpy(result, make_http_message(msg, NULL, NULL));
	}else if(strcmp(msg->method, "POST") == 0){
		if(fl != -1){
			if(S_ISDIR(st.st_mode)){
				if(msg->body[0] == 'q'){
					//query
					//변수 초기화
					int i;
					char input[BUF_SIZE];
					memset(input, 0, BUF_SIZE);

					//ls 결과값 받기
					strcpy(op, "ls -l ");
					strcat(op, path);
					parse_ls(system_func(op));

					strcpy(input, "[\n");
					//검색한 파일 목록의 이름에 키워드를 포함한 목록만 json으로 만듦
					char* tok = strtok(msg->body + 2, "+");
					for(i = 0; i < token_index; i++)
						if(strstr(token_table[i]->name, tok) != NULL){
							strcat(input, "{");
							strcat(input, "\"permision\":\"");
							strcat(input, token_table[i]->permission);
							strcat(input, "\",\"link\":\"");
							strcat(input, token_table[i]->link);
							strcat(input, "\",\"owner\":\"");
							strcat(input, token_table[i]->owner);
							strcat(input, "\",\"group\":\"");
							strcat(input, token_table[i]->group);
							strcat(input, "\",\"size\":\"");
							strcat(input, token_table[i]->size);
							strcat(input, "\",\"time\":\"");
							strcat(input, token_table[i]->time);
							strcat(input, "\",\"name\":\"");
							strcat(input, token_table[i]->name);
							strcat(input, "\"},\n");
						}

					while((tok = strtok(NULL, "+")) != NULL)
						for(i = 0; i < token_index; i++)
							if(strstr(token_table[i]->name, tok) != NULL){
								strcat(input, "{");
								strcat(input, "\"permision\":\"");
								strcat(input, token_table[i]->permission);
								strcat(input, "\",\"link\":\"");
								strcat(input, token_table[i]->link);
								strcat(input, "\",\"owner\":\"");
								strcat(input, token_table[i]->owner);
								strcat(input, "\",\"group\":\"");
								strcat(input, token_table[i]->group);
								strcat(input, "\",\"size\":\"");
								strcat(input, token_table[i]->size);
								strcat(input, "\",\"time\":\"");
								strcat(input, token_table[i]->time);
								strcat(input, "\",\"name\":\"");
								strcat(input, token_table[i]->name);
								strcat(input, "\"},\n");
							}
					strcat(input, "]");
					strcpy(result, make_http_message(msg, &st, input));
				}else if(msg->body != NULL)
					strcpy(result, make_http_message(msg, &st, NULL));
				else{
					//지역 변수 초기화
					FILE* fp = NULL;
					char file[path_len + 10];
					int i, index;
					memset(file, 0, path_len + 10);
					index = 0;

					//폴더 안에 item 이름이 붙은 파일 갯수를 셈
					strcpy(op, "ls -l ");
					strcat(op, path);
					parse_ls(system_func(op));
					for(i = 0; i < token_index; i++)
						if(strstr(token_table[i]->name, "item") != NULL)
							index++;

					//파일명을 itemindex로 만듦
					strcpy(file, path);
					strcat(file, "/item");
					char ptr_index[5];
					memset(ptr_index, 0, 5);
					sprintf(ptr_index, "%d", index);
					strcat(file, ptr_index);

					//파일에 메시지 body를 씀
					if((fp = fopen(file, "w")) != NULL){
						fprintf(fp, "%s", msg->body);
						fclose(fp);
					}

					strcpy(result, make_http_message(msg, &st, file));
				}
			}else if(S_ISREG(st.st_mode)){
				FILE* fp = NULL;
				char file[path_len];
				memset(file, 0, path_len);
				strcpy(file, path);

				//파일에 메시지 body를 이어씀
				if((fp = fopen(file, "a")) != NULL){
					fprintf(fp, "%s", msg->body);
					fclose(fp);
				}

				strcpy(result, make_http_message(msg, &st, NULL));
			}

		}else
			strcpy(result, make_http_message(msg, NULL, NULL));

	}else if(strcmp(msg->method, "PUT") == 0){
		FILE* fp = NULL;

		if(msg->body == NULL){
			if(fl == -1)
				mkdir(path, 0755);
			strcpy(result, make_http_message(msg, NULL, path));

		}else if(fl != -1 && S_ISDIR(st.st_mode))
			//이미 폴더가 존재하면
			strcpy(result, make_http_message(msg, &st, NULL));

		else if((fp = fopen(path, "w")) != NULL){
			fprintf(fp, "%s", msg->body);
			fclose(fp);
			if(fl == -1)
				strcpy(result, make_http_message(msg, NULL, NULL));
			else
				strcpy(result, make_http_message(msg, &st, NULL));
		}
	}else if(strcmp(msg->method, "DELETE") == 0){
		if(fl != -1){
			strcpy(op, "rm -rf ");
			strcat(op, path);
			system(op);
			strcpy(result, make_http_message(msg, &st, NULL));
		}else
			strcpy(result, make_http_message(msg, NULL, NULL));
	}else{
		free(path);
		free(op);
		return NULL;
	}
	free(path);
	free(op);
	return result;
}

/*
 * 클라이언트에 보낼 메시지를 리턴하는 함수
 */
char* make_http_message(http_message* msg, struct stat* ptr_st, char* input){
	//변수 초기화
	struct stat st;
	if(ptr_st != NULL)
		st = *ptr_st;
	char result[BUF_SIZE];
	memset(result, 0, BUF_SIZE);

	if(msg == NULL && ptr_st == NULL && input == NULL){
		//메시지 파싱에 실패한 경우
		strcpy(result, "HTTP/1.1 400");
	}else if(strcmp(msg->method, "GET") == 0){
		if(ptr_st != NULL){
			strcpy(result, "HTTP/1.1 200 OK\n");
			if(S_ISDIR(st.st_mode))
				//폴더 내 파일 목록
				strcat(result, "Content-Type:application/json\n\n");
			else if(S_ISREG(st.st_mode))
				//파일 내용
				strcat(result, "Content-Type:text/plain\n\n");
			strcat(result, input);
		}else
			//파일 혹은 폴더가 존재하지 않는 경우
			strcpy(result, "HTTP/1.1 404");
	}else if(strcmp(msg->method, "POST") == 0){
		if(ptr_st != NULL){
			strcpy(result, "HTTP/1.1 ");
			if(S_ISDIR(st.st_mode)){
				if(msg->body[0] == 'q'){
					//query
					strcat(result, "200 OK\nContent-Type:application/json\n\n");
					strcat(result, input);
				}else if(msg->body != NULL)
					//폴더에 이어쓰기 시도
					strcat(result, "405");
				else{
					//폴더 내 파일 생성
					strcat(result, "201 Created\nLocation:");
					strcat(result, msg->host);
					strcat(result, input);
					strcat(result, "\n\n");
					strcat(result, msg->body);
				}
			}else if(S_ISREG(st.st_mode))
				//파일 이어쓰기
				strcat(result, "0 OK");
		}else
			//파일 혹은 폴더가 존재하지 않는 경우
			strcpy(result, "HTTP/1.1 404");

	}else if(strcmp(msg->method, "PUT") == 0){
		strcpy(result, "HTTP/1.1 ");

		if(ptr_st == NULL){
			strcat(result, "201 Created\n");
			if(msg->body != NULL)
				//파일 생성
				strcat(result, "Content-Type:text/plain\n\n");
			else{
				//폴더 생성
				strcat(result, "Location:");
				strcat(result, msg->host);
				strcat(result, input);
			}
		}else{
			if(msg->body != NULL){
				//파일 덮어쓰기
				strcat(result, "200 OK\n");
				strcat(result, "Content-Type:text/plain\n\n");
			}else
				//폴더에 덮어쓰기를 시도한 경우
				strcat(result, "405");
		}

		if(msg->body != NULL)
			strcat(result, msg->body);
		
	}else if(strcmp(msg->method, "DELETE") == 0){
		strcpy(result, "HTTP/1.1 ");
		if(ptr_st == NULL)
			//파일 혹은 폴더가 존재하지 않는 경우
			strcat(result, "404");
		else
			//파일 혹은 폴더를 삭제
			strcat(result, "200 OK");

	}else
		//이외의 오류
		strcpy(result, "HTTP/1.1 400");

	strcat(result, "\n");
	return result;
}

/*
 * ls 명령어로 얻은 목록을 token_table에 파싱하는 함수
 */
void parse_ls(char* ls){
	//변수 초기화
	int i, prev_i, count, time_count;
	char* tok_line;
	char* tmp = NULL;
	ls_token* token;

	//이전 토큰 제거
	for(i = 0; i < token_index; i++)
		free(token_table[i]);

	//한 줄씩 파싱
	tok_line = strtok(ls, "\n");
	token_index = 0;
	while((tok_line = strtok(NULL, "\n")) != NULL){
		//변수 초기화
		prev_i = 0;
		count = 0;
		time_count = 0;
		token = (ls_token*) malloc(sizeof(ls_token));
		token->permission = 0;
		token->link = 0;
		token->owner = 0;
		token->group = 0;
		token->size = 0;
		token->time = 0;
		token->name = 0;

		for(i = 0; i < strlen(tok_line); i++){
			if(tok_line[i] == ' ' || i == strlen(tok_line) - 1){
				if(count < 4)
					tmp = strndup(tok_line + prev_i, i - prev_i);

				switch(count){
					case 0:
						token->permission = tmp;
						prev_i = i + 1;
						count++;
						break;
					case 1:
						token->link = tmp;
						prev_i = i + 1;
						count++;
						break;
					case 2:
						token->owner = tmp;
						prev_i = i + 1;
						count++;
						break;
					case 3:
						token->group = tmp;
						prev_i = i + 1;
						count++;
						break;
					case 4:
						if(prev_i == i)
							//이어진 공백 무시
							prev_i++;
						else{
							tmp = strndup(tok_line + prev_i, i - prev_i);
							token->size = tmp;
							prev_i = i + 1;
							count++;
						}
						break;
					case 5:
						if(time_count < 2){
							if(prev_i == i)
								//첫 문자열의 이어진 공백 무시
								prev_i++;
							else if(tok_line[i + 1] != ' ')
								//첫 문자열 이후의 문자열에서 이어진 공백 무시
								time_count++;
						}else{
							//세번째 문자열에 도달하면 파싱
							tmp = strndup(tok_line + prev_i, i - prev_i);
							token->time = tmp;
							prev_i = i + 1;
							count++;
						}
						break;
					case 6:
						if(i == strlen(tok_line) - 1){
							i++;
							tmp = strndup(tok_line + prev_i, i - prev_i);
							token->name = tmp;
							count++;
						}
						break;
					default:
						break;
				}
			}
		}

		token_table[token_index++] = token;
	}

}

/*
 * 파싱된 token_table 목록들을 json 형식 문자열로 리턴하는 함수
 */
char* to_json(){
	char result[BUF_SIZE];
	memset(result, 0, BUF_SIZE);
	strcpy(result, "[\n");
	int i;
	i = 0;
	for(; i < token_index; i++){
		strcat(result, "{");
		strcat(result, "\"permision\":\"");
		strcat(result, token_table[i]->permission);
		strcat(result, "\",\"link\":\"");
		strcat(result, token_table[i]->link);
		strcat(result, "\",\"owner\":\"");
		strcat(result, token_table[i]->owner);
		strcat(result, "\",\"group\":\"");
		strcat(result, token_table[i]->group);
		strcat(result, "\",\"size\":\"");
		strcat(result, token_table[i]->size);
		strcat(result, "\",\"time\":\"");
		strcat(result, token_table[i]->time);
		strcat(result, "\",\"name\":\"");
		strcat(result, token_table[i]->name);
		strcat(result, "\"},\n");

	}
	strcat(result, "]");
	return result;
}
