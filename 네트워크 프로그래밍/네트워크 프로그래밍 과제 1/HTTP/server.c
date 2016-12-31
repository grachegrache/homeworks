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

//HTTP �޽��� ����ü
typedef struct {
	char* method;
	char* operand;
	char* host;
	char* body;
} http_message;

//ls ��� ����ü
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
	//���� �ʱ�ȭ
	int serv_sock, clnt_sock, optlen, option;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	char buf[BUF_SIZE];
	char message[BUF_SIZE];
	char* port = "10000";
	ssize_t ret;
	memset(message, 0, sizeof(message));

	//���� ���� �����
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

		// http_message �Ľ�
		http_message* msg = get_http_method(buf);

		// http message ����
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
 * ������ �͹̳� ��ɾ �����ϰ� ����� �����ϴ� �Լ�
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
 * ���ڿ��� HTTP �޽��� ����ü�� �Ľ��ϴ� �Լ�
 */
http_message* get_http_method(char* buf){
	//���� �ʱ�ȭ
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
			//�ֵ���ǥ�� ������ ��ĭ�� �����ϰ�
			//���� �ֵ���ǥ�� ���� ������ ����
			if(prev_i < i && buf[i] == '\"'){
				tmp = strndup(buf + prev_i + 1, i - prev_i - 1);
				msg->operand = tmp;
				i++;
				count++;
			}
		}
		
		if(buf[i] == ' ' || buf[i] == '\n' || i == strlen(buf) - 1){
			//��ĭ, ����, ������ ���ڿ� ������
			
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
 * �Ľ̵� HTTP �޽����� ���� ������ �����ϴ� �Լ�
 * Ŭ���̾�Ʈ�� ���� �޽����� �����Ѵ�
 */
char* run_http_method(http_message* msg){
	if(msg->operand == NULL)
		return NULL;
	//���� �ʱ�ȭ
	char result[BUF_SIZE];
	char input[BUF_SIZE];
	memset(result, 0, BUF_SIZE);
	memset(input, 0, BUF_SIZE);

	//home directory ��� ���
	struct passwd* user_pw;
	user_pw = getpwuid(getuid());
	char* homedir = user_pw->pw_dir;

	//path �����
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

	//���� ���� ��������
	struct stat st;
	int fl = stat(path, &st);
	if(fl == -1)
		perror("stat");

	//��ɾ� �ʱ�ȭ
	char* op = (char*) calloc(strlen(path) + 10, sizeof(char));
	memset(op, 0, strlen(path) + 10);

	if(strcmp(msg->method, "GET") == 0){
		if(fl != -1){
			if(S_ISDIR(st.st_mode)){
				//�����̸�
				strcpy(op, "ls -l ");
				strcat(op, path);
				parse_ls(system_func(op));
				strcpy(input, to_json());
			}else if(S_ISREG(st.st_mode)){
				//�����̸�
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
					//���� �ʱ�ȭ
					int i;
					char input[BUF_SIZE];
					memset(input, 0, BUF_SIZE);

					//ls ����� �ޱ�
					strcpy(op, "ls -l ");
					strcat(op, path);
					parse_ls(system_func(op));

					strcpy(input, "[\n");
					//�˻��� ���� ����� �̸��� Ű���带 ������ ��ϸ� json���� ����
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
					//���� ���� �ʱ�ȭ
					FILE* fp = NULL;
					char file[path_len + 10];
					int i, index;
					memset(file, 0, path_len + 10);
					index = 0;

					//���� �ȿ� item �̸��� ���� ���� ������ ��
					strcpy(op, "ls -l ");
					strcat(op, path);
					parse_ls(system_func(op));
					for(i = 0; i < token_index; i++)
						if(strstr(token_table[i]->name, "item") != NULL)
							index++;

					//���ϸ��� itemindex�� ����
					strcpy(file, path);
					strcat(file, "/item");
					char ptr_index[5];
					memset(ptr_index, 0, 5);
					sprintf(ptr_index, "%d", index);
					strcat(file, ptr_index);

					//���Ͽ� �޽��� body�� ��
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

				//���Ͽ� �޽��� body�� �̾
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
			//�̹� ������ �����ϸ�
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
 * Ŭ���̾�Ʈ�� ���� �޽����� �����ϴ� �Լ�
 */
char* make_http_message(http_message* msg, struct stat* ptr_st, char* input){
	//���� �ʱ�ȭ
	struct stat st;
	if(ptr_st != NULL)
		st = *ptr_st;
	char result[BUF_SIZE];
	memset(result, 0, BUF_SIZE);

	if(msg == NULL && ptr_st == NULL && input == NULL){
		//�޽��� �Ľ̿� ������ ���
		strcpy(result, "HTTP/1.1 400");
	}else if(strcmp(msg->method, "GET") == 0){
		if(ptr_st != NULL){
			strcpy(result, "HTTP/1.1 200 OK\n");
			if(S_ISDIR(st.st_mode))
				//���� �� ���� ���
				strcat(result, "Content-Type:application/json\n\n");
			else if(S_ISREG(st.st_mode))
				//���� ����
				strcat(result, "Content-Type:text/plain\n\n");
			strcat(result, input);
		}else
			//���� Ȥ�� ������ �������� �ʴ� ���
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
					//������ �̾�� �õ�
					strcat(result, "405");
				else{
					//���� �� ���� ����
					strcat(result, "201 Created\nLocation:");
					strcat(result, msg->host);
					strcat(result, input);
					strcat(result, "\n\n");
					strcat(result, msg->body);
				}
			}else if(S_ISREG(st.st_mode))
				//���� �̾��
				strcat(result, "0 OK");
		}else
			//���� Ȥ�� ������ �������� �ʴ� ���
			strcpy(result, "HTTP/1.1 404");

	}else if(strcmp(msg->method, "PUT") == 0){
		strcpy(result, "HTTP/1.1 ");

		if(ptr_st == NULL){
			strcat(result, "201 Created\n");
			if(msg->body != NULL)
				//���� ����
				strcat(result, "Content-Type:text/plain\n\n");
			else{
				//���� ����
				strcat(result, "Location:");
				strcat(result, msg->host);
				strcat(result, input);
			}
		}else{
			if(msg->body != NULL){
				//���� �����
				strcat(result, "200 OK\n");
				strcat(result, "Content-Type:text/plain\n\n");
			}else
				//������ ����⸦ �õ��� ���
				strcat(result, "405");
		}

		if(msg->body != NULL)
			strcat(result, msg->body);
		
	}else if(strcmp(msg->method, "DELETE") == 0){
		strcpy(result, "HTTP/1.1 ");
		if(ptr_st == NULL)
			//���� Ȥ�� ������ �������� �ʴ� ���
			strcat(result, "404");
		else
			//���� Ȥ�� ������ ����
			strcat(result, "200 OK");

	}else
		//�̿��� ����
		strcpy(result, "HTTP/1.1 400");

	strcat(result, "\n");
	return result;
}

/*
 * ls ��ɾ�� ���� ����� token_table�� �Ľ��ϴ� �Լ�
 */
void parse_ls(char* ls){
	//���� �ʱ�ȭ
	int i, prev_i, count, time_count;
	char* tok_line;
	char* tmp = NULL;
	ls_token* token;

	//���� ��ū ����
	for(i = 0; i < token_index; i++)
		free(token_table[i]);

	//�� �پ� �Ľ�
	tok_line = strtok(ls, "\n");
	token_index = 0;
	while((tok_line = strtok(NULL, "\n")) != NULL){
		//���� �ʱ�ȭ
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
							//�̾��� ���� ����
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
								//ù ���ڿ��� �̾��� ���� ����
								prev_i++;
							else if(tok_line[i + 1] != ' ')
								//ù ���ڿ� ������ ���ڿ����� �̾��� ���� ����
								time_count++;
						}else{
							//����° ���ڿ��� �����ϸ� �Ľ�
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
 * �Ľ̵� token_table ��ϵ��� json ���� ���ڿ��� �����ϴ� �Լ�
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
