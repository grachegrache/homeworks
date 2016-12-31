/*
 * server.c
 *
 *  Created on: 2015. 6. 4.
 *      Author: panda
 */

#include "server.h"

int pipe_serv[2];
int pipe_scrt[2];
int m_fd = -1;

//SHARE MEMORY ����
game_info* game;

int main(int argc, char *argv[]){
	int serv_sock, secret_sock, optlen, option1, option2, str_len, sig_state;
	struct sockaddr_in serv_adr, secret_adr;
	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	char buf[BUF_SIZE];
	char send_buf[BUF_SIZE];
	char* main_port = "10000";
	char* secret_port = "10001";
	memset(buf, 0, BUF_SIZE);
	memset(send_buf, 0, BUF_SIZE);

	//game_info �ʱ�ȭ
	initialize_mmap();
	game = (game_info*) malloc(sizeof(game_info));
	game = reset_game();
	if(game == NULL)
		return -1;

	if(argc == 2)
		main_port = argv[1];

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sig_state = sigaction(SIGCHLD, &act, 0);

	//main server ���� �����
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(main_port));

	//secret server ���� �����
	secret_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&secret_adr, 0, sizeof(secret_adr));
	secret_adr.sin_family = AF_INET;
	secret_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	secret_adr.sin_port = htons(atoi(secret_port));

	//set REUSEADDR
	optlen = sizeof(option1);
	option1 = true;
	option2 = true;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*) &option1, optlen);
	setsockopt(secret_sock, SOL_SOCKET, SO_REUSEADDR, (void*) &option2, optlen);

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	if(bind(secret_sock, (struct sockaddr*) &secret_adr, sizeof(secret_adr)) == -1)
		error_handling("bind() error");
	if(listen(secret_sock, 5) == -1)
		error_handling("listen() error");

	//pipe ���� �� game->clnt �ʱ�ȭ
	pipe(pipe_serv);
	pipe(pipe_scrt);
	reset_clnt();

	//�ñ׳� ���
	signal(SIGALRM, handler_arlam);
	signal(SIGUSR1, handler_usr1);
	signal(SIGUSR2, handler_usr2);

	pid = fork();
	if(pid == -1)
		return -1;

	if(pid == 0){
		//��� ä�ù� ����
		game = get_mmap();
		close(serv_sock);

		while(true){
			struct sockaddr clnt_adr;
			adr_sz = sizeof(clnt_adr);
			int actual = accept(secret_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
//			mlock(game, sizeof(game));
			game->clnt_scrt[game->clnt_scrt_num].sock = actual;
//			munlock(game, sizeof(game));

			printf("actual sock: %d\n", actual);
			printf("��� ä�ù� sock: %d\n", game->clnt_scrt[game->clnt_scrt_num].sock);

			if(game->clnt_scrt_num >= CLIENT_MAX){
				char message[] = "Error\nFull of Clients!";
				write(game->clnt_scrt[game->clnt_scrt_num].sock, message, sizeof(message));
				close(game->clnt_scrt[game->clnt_scrt_num].sock);
				continue;
			}

			if(game->clnt_scrt[game->clnt_scrt_num].sock == -1)
				continue;
			else
				puts("new secret client connected...");

			pid = fork();
			if(pid == -1){
				close(game->clnt_scrt[game->clnt_scrt_num].sock);
				continue;
			}

			if(pid == 0){
				//�ڽ� ���μ���
				close(secret_sock);
				game = get_mmap();
				int index = game->clnt_scrt_num++;
				message* msg;

				printf("��� ä�ù� Ŭ���̾�Ʈ %d, sock %d\n", index, game->clnt_scrt[index].sock);

				while(true){
					str_len = read(game->clnt_scrt[index].sock, buf, BUF_SIZE);

					printf("��� ä�ù� ��û %d, %s\n", game->clnt_scrt[index].sock, buf);

					if(str_len > 0){
						if(strstr(buf, TYPE_START) != NULL)
							continue;

						else if(strstr(buf, TYPE_CHOICE) != NULL)
							continue;

						else if((msg = parse_message(buf)) == NULL){
							char message[] = "Error\nWrong Message!";
							write(game->clnt_scrt[index].sock, message, sizeof(message));

						}else{
							if(strcmp(msg->type, TYPE_ACCESS) == 0){
								strcpy(game->clnt_scrt[index].name, msg->contents);

								strcpy(send_buf, "Accept\n");
								strcat(send_buf, msg->contents);

							}else if(strcmp(msg->type, TYPE_CHAT) == 0){
								strcpy(send_buf, "Chatted\n");
								strcat(send_buf, game->clnt_scrt[index].name);
								strcat(send_buf, "/");
								strcat(send_buf, msg->contents);

							}else if(strcmp(msg->type, TYPE_INVITE) == 0){
								strcpy(send_buf, "Invited\n");
								strcat(send_buf, msg->contents);

							}else if(strcmp(msg->type, TYPE_EXIT) == 0){
								strcpy(send_buf, "Exited\n");
								strcat(send_buf, game->clnt_scrt[index].name);
							}

							if(strstr(send_buf, "Error") != NULL)
								write(game->clnt[index].sock, send_buf, sizeof(send_buf));

							else{
								while(game->handling_usr2)
									;
								write(pipe_scrt[1], send_buf, sizeof(send_buf));
								kill(getppid(), SIGUSR2);

								if(strcmp(msg->type, TYPE_EXIT) == 0)
									break;

							}
							memset(send_buf, 0, BUF_SIZE);
						}
					}
					memset(buf, 0, BUF_SIZE);
				}

				while(game->handling_usr2)
					;
				memset(send_buf, 0, BUF_SIZE);
				remove_scrt_clnt(index);

				puts("secret client disconnected...");
				munmap(game, sizeof(game));
				return 0;
			}
		}

		close(secret_sock);
		munmap(game, sizeof(game));
		return 0;

	}else{
		//���� ä�ù� ����
		close(secret_sock);

		while(true){
			struct sockaddr clnt_adr;
			adr_sz = sizeof(clnt_adr);
			int actual = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
//			mlock(game, sizeof(game));
			game->clnt[game->clnt_num].sock = actual;
//			munlock(game, sizeof(game));

			printf("actual sock: %d\n", actual);
			printf("���� ä�ù� sock: %d\n", game->clnt[game->clnt_num].sock);

			if(game->clnt_num >= CLIENT_MAX){
				char message[] = "Error\nFull of Clients!";
				write(game->clnt[game->clnt_num].sock, message, sizeof(message));
				close(game->clnt[game->clnt_num].sock);
				continue;
			}

			if(game->clnt[game->clnt_num].sock == -1)
				continue;
			else
				puts("new client connected...");

			pid = fork();
			if(pid == -1){
				close(game->clnt[game->clnt_num].sock);
				continue;
			}

			if(pid == 0){
				//�ڽ� ���μ���
				game = get_mmap();
				close(serv_sock);

				int index = game->clnt_num++;
				message* msg;

				printf("���� ä�ù� Ŭ���̾�Ʈ %d, sock %d\n", index, game->clnt[index].sock);

				while(true){
					str_len = read(game->clnt[index].sock, buf, BUF_SIZE);
					if(str_len == 0)
						break;
					printf("���� ä�ù� ��û %d, sock %d\n%s\n", index, game->clnt[index].sock, buf);

					if(str_len > 0){
						if((msg = parse_message(buf)) == NULL){
							char message[] = "Error\nWrong Message!";
							write(game->clnt[index].sock, message, sizeof(message));

						}else{
							char* tmp = make_message(msg, index);
							if(tmp == NULL || strlen(tmp) == 0)
								continue;

							strcpy(send_buf, tmp);

							if(strstr(send_buf, "Error") != NULL)
								write(game->clnt[index].sock, send_buf, sizeof(send_buf));

							else{
								while(game->handling_usr1)
									;
								write(pipe_serv[1], send_buf, sizeof(send_buf));
								kill(getppid(), SIGUSR1);

								printf("send_buf\n\"%s\"\n", send_buf);

								if(strstr(send_buf, "Exited") != NULL){
									break;
									
								}else if(strstr(send_buf, "Started") != NULL){
									//���� ���� ���� Ȯ��
									if(game->clnt_num >= CLIENT_MIN){
										int total_start = 1;
										int i;
										for(i = 0; i < game->clnt_num; i++)
											total_start &= game->clnt[i].start;

										if(total_start){
											//���Ǿ� ����
											time_t current_time;
											time(&current_time);
											srand(current_time);
											int first_mafia = rand() % game->clnt_num;
											int second_mafia;

											game->clnt[first_mafia].mafia = 1;

											if(game->clnt_num > 6){
												while((second_mafia = rand() % game->clnt_num) == first_mafia)
													;

												game->clnt[second_mafia].mafia = 1;

											}

											//���� ���� ī��Ʈ�ٿ�
											alarm(0);
											alarm(3);
										}
									}

								}else if(strstr(send_buf, "Choosen") != NULL){
									printf("choosen\n");

									if(game->voted){
										//��ǥ �Ϸ�
										printf("voted\n");
										game->voted = 0;
										if(game->mafia_only){
											game->mafia_only = 0;
											game->state = STATE_DAY;
										}else{
											game->mafia_only = 1;
											game->state = STATE_NIGHT;
										}

										if(strcmp(game->state, STATE_NIGHT) == 0)
											if(game->clnt_num < 6){
												//���Ǿư� 1���̸�
												game->state = STATE_VOTE;

											}

										if(!check_gameover()){

											memset(send_buf, 0, BUF_SIZE);
											strcpy(send_buf, "State\n");
											strcat(send_buf, game->state);

											while(game->handling_usr1)
												;
											write(pipe_serv[1], send_buf, sizeof(send_buf));
											kill(getppid(), SIGUSR1);

											alarm(0);
											alarm(TIME_CHAT);
										}

									}else if(game->revote){
										//����ǥ
										game->revote = 0;
										alarm(0);
										alarm(TIME_VOTE);
									}

								}
							}
							memset(send_buf, 0, BUF_SIZE);
						}
					}
					memset(buf, 0, BUF_SIZE);
				}

				while(game->handling_usr1)
					;
				memset(send_buf, 0, BUF_SIZE);
				remove_clnt(index);

				check_gameover();
				puts("client disconnected...");
				munmap(game, sizeof(game));
				return 0;
			}

		}

		close(serv_sock);
		munmap(game, sizeof(game));
		close(m_fd);
		return 0;
	}

}

void initialize_mmap(){
	int i;
	FILE* fp = fopen("mmap_file", "w+");
	for(i = 0; i < sizeof(game_info); i++)
		fprintf(fp, "%s", "0");
	fclose(fp);
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_childproc(int sig){
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d \n", pid);
}

void handler_usr1(int sig){
	game->handling_usr1 = 1;

	int str_len, i;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	str_len = read(pipe_serv[0], buf, BUF_SIZE);

	printf("�ڵ鷯 ������\n%s\n", buf);

	if(str_len > 0){
		if(game->mafia_only){
			for(i = 0; i < game->clnt_num; i++)
				if(game->clnt[i].mafia)
					write(game->clnt[i].sock, buf, str_len);
		}else{
			for(i = 0; i < game->clnt_num; i++)
				write(game->clnt[i].sock, buf, str_len);
		}
	}

	if(game->start && strstr(buf, STATE_DAY) != NULL){
		//���� ����
		game->start = 0;

		for(i = 0; i < game->clnt_num; i++){
			//�������� �޽��� �����
			memset(buf, 0, BUF_SIZE);
			strcpy(buf, "Chatted\nServer/");
			strcat(buf, "����\n����� ");
			if(game->clnt[i].mafia)
				strcat(buf, "���Ǿ�");
			else
				strcat(buf, "�ù�");
			strcat(buf, "�Դϴ�.");

			write(game->clnt[i].sock, buf, sizeof(buf));
		}
	}

	game->handling_usr1 = 0;
}

void handler_usr2(int sig){
	game->handling_usr2 = 1;

	int str_len, i;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	str_len = read(pipe_scrt[0], buf, BUF_SIZE);

	printf("�ڵ鷯 ��� ������\n%s\n", buf);

	if(str_len > 0)
		for(i = 0; i < game->clnt_scrt_num; i++)
			write(game->clnt_scrt[i].sock, buf, str_len);

	game->handling_usr2 = 0;
}

void handler_arlam(int sig){
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);
	//��ǥ �Ϸ� �� �� �˶� ����
	if(game->voted)
		return;

	//����ǥ �� �� �˶� ����
	if(game->revote)
		return;

	//GEnd�� ���� �������� �˻�
	int i, is_mafia;
	is_mafia = 0;
	for(i = 0; i < game->clnt_num; i++)
		is_mafia |= game->clnt[i].mafia;

	//GEnd �� �� �˶� ����
	if(is_mafia == 0)
		return;

	if(strcmp(game->state, STATE_READY) == 0){
		//ī��Ʈ�� ������

		game->state = STATE_DAY;

		strcpy(buf, "State\n");
		strcat(buf, STATE_DAY);

		while(game->handling_usr1)
			;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);

		alarm(0);
		alarm(TIME_CHAT);

	}else if(strcmp(game->state, STATE_DAY) == 0){
		//���� ������

		game->state = STATE_VOTE;

		strcpy(buf, "State\n");
		strcat(buf, STATE_VOTE);

		while(game->handling_usr1)
			;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);

		alarm(0);
		alarm(TIME_VOTE);

	}else if(strcmp(game->state, STATE_NIGHT) == 0){
		//���� ������

		game->state = STATE_VOTE;

		strcpy(buf, "State\n");
		strcat(buf, STATE_VOTE);

		while(game->handling_usr1)
			;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);

		alarm(0);
		alarm(TIME_VOTE);
		
	}else if(strcmp(game->state, STATE_VOTE) == 0){
		//��ǥ�� �ð��������� ������
		if(game->mafia_only){
			//���Ǿ� ��ǥ�� ������
			game->mafia_only = 0;
			game->state = STATE_DAY;
		}else{
			//�ù�+���Ǿ� ��ǥ�� ������
			game->mafia_only = 1;
			game->state = STATE_NIGHT;
		}

		if(strcmp(game->state, STATE_NIGHT) == 0)
			if(game->clnt_num < 6){
				//���Ǿư� 1���̸�
				game->state = STATE_VOTE;

			}
		strcpy(buf, "State\n");
		strcat(buf, game->state);

		while(game->handling_usr1)
			;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);

		if(strcmp(game->state, STATE_VOTE) == 0){
			alarm(0);
			alarm(TIME_VOTE);
			return;
		}

		alarm(0);
		alarm(TIME_CHAT);
	}
}

game_info* reset_game(){
	game_info* game;

	game = get_mmap();
	game->clnt_num = 0;
	game->clnt_scrt_num = 0;
	game->state = STATE_READY;
	game->mafia_only = 0;
	game->start = 1;
	game->voted = 0;
	game->revote = 0;
	game->handling_usr1 = 0;
	game->handling_usr2 = 0;

	return game;
}

void reset_clnt(){
	int i;

	for(i = 0; i < CLIENT_MAX; i++){
		memset(game->clnt[i].name, 0, 10);
		game->clnt[i].sock = NULL;
		game->clnt[i].start = NULL;
		game->clnt[i].voted = NULL;
		game->clnt[i].killed = NULL;
		game->clnt[i].mafia = NULL;
	}

	game->clnt_num = 0;

}

int get_index(char* username){
	int i;
	for(i = 0; i < game->clnt_num; i++)
		if(strcmp(game->clnt[i].name, username) == 0)
			return i;
	return -1;
}

game_info* get_mmap(){
	if(m_fd < 0)
		if(0 > (m_fd = open("mmap_file", O_RDWR | O_CREAT))){
			printf("���ۿ� ������ �� �����ϴ�.\n");
			return 0;
		}

	return (game_info*) mmap(0, sizeof(game_info), PROT_WRITE | PROT_READ, MAP_SHARED, m_fd, 0);
}

void remove_clnt(int index){
	int i;
	close(game->clnt[index].sock);

	for(i = index; i < game->clnt_num - 1; i++){
		game->clnt[i].sock = game->clnt[i + 1].sock;
		strcpy(game->clnt[i].name, game->clnt[i + 1].name);
		game->clnt[i].start = game->clnt[i + 1].start;
		game->clnt[i].voted = game->clnt[i + 1].voted;
		game->clnt[i].killed = game->clnt[i + 1].killed;
		game->clnt[i].mafia = game->clnt[i + 1].mafia;
	}
	game->clnt[i].sock = NULL;
	memset(game->clnt[i].name, 0, 10);
	game->clnt[i].start = NULL;
	game->clnt[i].voted = NULL;
	game->clnt[i].killed = NULL;
	game->clnt[i].mafia = NULL;

	game->clnt_num--;

}

void remove_scrt_clnt(int index){
	int i;
	close(game->clnt_scrt[index].sock);

	for(i = index; i < game->clnt_scrt_num - 1; i++){
		game->clnt_scrt[i].sock = game->clnt_scrt[i + 1].sock;
		strcpy(game->clnt_scrt[i].name, game->clnt_scrt[i + 1].name);
		game->clnt_scrt[i].start = game->clnt_scrt[i + 1].start;
		game->clnt_scrt[i].voted = game->clnt_scrt[i + 1].voted;
		game->clnt_scrt[i].killed = game->clnt_scrt[i + 1].killed;
		game->clnt_scrt[i].mafia = game->clnt_scrt[i + 1].mafia;
	}
	game->clnt_scrt[i].sock = NULL;
	memset(game->clnt_scrt[i].name, 0, 10);
	game->clnt_scrt[i].start = NULL;
	game->clnt_scrt[i].voted = NULL;
	game->clnt_scrt[i].killed = NULL;
	game->clnt_scrt[i].mafia = NULL;

	game->clnt_scrt_num--;

}

message* parse_message(char* buf){
	if(buf == NULL || strlen(buf) == 0)
		return NULL;

	char* cpy = strdup(buf);
	message* msg = (message*) malloc(sizeof(message));
	msg->type = NULL;
	msg->contents = NULL;

	char* tok = strtok(cpy, "\n");
	if((strcmp(tok, TYPE_ACCESS) && strcmp(tok, TYPE_START) && strcmp(tok, TYPE_CHAT) && strcmp(tok,
	TYPE_CHOICE) && strcmp(tok, TYPE_EXIT)) != 0){
		free(cpy);
		free(msg);
		return NULL;
	}

	msg->type = strdup(tok);

	if(strcmp(msg->type, TYPE_START) != 0)
		if((tok = strtok(NULL, "\n")) != NULL){
			msg->contents = strdup(tok);
			if(msg->contents[strlen(tok) - 1] == '\n')
				msg->contents[strlen(tok) - 1] = 0;
		}

	return msg;
}

char* make_message(message* msg, int index){
	int i;
	char message[BUF_SIZE];
	memset(message, 0, BUF_SIZE);

	if(strcmp(msg->type, TYPE_ACCESS) == 0){
		if(strcmp(game->state, STATE_READY) == 0){
			int exist = 0;
			//�����ϴ� �̸����� Ȯ��
			for(i = 0; i < game->clnt_num; i++)
				if(game->clnt[i].name != NULL && i != index)
					if(strcmp(msg->contents, game->clnt[i].name) == 0){
						strcpy(message, "Error\nDuplicated name!");
						exist = 1;
					}

			if(!exist){
				strcpy(game->clnt[index].name, msg->contents);
				strcpy(message, "Accept\n");
				strcat(message, msg->contents);
			}
		}else
			strcpy(message, "Error\nNot a Ready State!");

	}else if(strcmp(msg->type, TYPE_START) == 0){
		if(strcmp(game->state, STATE_READY) == 0){

			game->clnt[index].start = 1;

			strcpy(message, "Started\n");
			strcat(message, game->clnt[index].name);

		}else
			strcpy(message, "Error\nNot a Ready State!");

	}else if(strcmp(msg->type, TYPE_CHAT) == 0){
		if(strcmp(game->state, STATE_DAY) == 0){
			strcpy(message, "Chatted\n");
			strcat(message, game->clnt[index].name);
			strcat(message, "/");
			strcat(message, msg->contents);

		}else if(strcmp(game->state, STATE_NIGHT) == 0){
			if(game->clnt[index].mafia){
				strcpy(message, "Chatted\n");
				strcat(message, game->clnt[index].name);
				strcat(message, "/");
				strcat(message, msg->contents);

			}else
				strcpy(message, "Error\nNot a Day State!");

		}else
			strcpy(message, "Error\nNot a Day or Night State!");

	}else if(strcmp(msg->type, TYPE_CHOICE) == 0){
		if(strcmp(game->state, STATE_VOTE) == 0){
			//��ǥ ����
			int vote_index = get_index(msg->contents);
			if(vote_index == -1)
				strcpy(message, "Error\nCan not find user name!");

			else{
				game->clnt[vote_index].voted++;

				int total_vote = 0;
				int choosen_index = 0;
				int same = 0;
				int voter;

				//��ǥ�� ����
				if(game->mafia_only){
					voter = 0;
					for(i = 0; i < game->clnt_num; i++)
						if(game->clnt[i].mafia)
							voter++;
				}else{
					voter = game->clnt_num;
					for(i = 0; i < game->clnt_num; i++)
						if(game->clnt[i].killed)
							voter--;
				}

				//�ִ� ��ǥ�� ����
				for(i = 0; i < game->clnt_num; i++){
					total_vote += game->clnt[i].voted;

					if(game->clnt[choosen_index].voted < game->clnt[i].voted)
						choosen_index = i;
				}

				//���� ���� Ȯ��
				for(i = 0; i < game->clnt_num; i++)
					if(choosen_index != i)
						if(game->clnt[choosen_index].voted == game->clnt[i].voted){
							same = 1;
							break;
						}

				//���� ��ǥ �Ϸ��ϸ�
				if(total_vote == voter){
					strcpy(message, "Choosen\n");
					if(same){
						game->revote = 1;
						strcat(message, "Fail");

					}else{
						game->voted = 1;
						game->revote = 0;
						game->clnt[choosen_index].killed = 1;
						strcat(message, game->clnt[choosen_index].name);
					}

					//Ŭ���̾�Ʈ voted �ʱ�ȭ
					for(i = 0; i < game->clnt_num; i++)
						game->clnt[i].voted = 0;

				}
			}

		}else
			strcpy(message, "Error\nNot a Vote State!");

	}else if(strcmp(msg->type, TYPE_EXIT) == 0){
		strcpy(message, "Exited\n");
		strcat(message, game->clnt[index].name);
	}

	return message;
}

int check_gameover(){
	int i;
	int count_citizen = 0;
	int count_mafia = 0;
	int last_mafia = 0;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	//��� �ִ� ���Ǿ�, �ù� �� ����
	for(i = 0; i < game->clnt_num; i++){
		if(game->clnt[i].mafia)
			last_mafia = i;
		
		if(!game->clnt[i].killed)
			if(game->clnt[i].mafia)
				count_mafia++;
			else if(!game->clnt[i].mafia)
				count_citizen++;
	}

	int isEnd = 0;
	if(count_mafia == 0){
		//�ù� �¸�
		isEnd = 1;
		strcpy(buf, "GEnd\nCitizen\n");

	}else if(count_mafia >= count_citizen){
		//���Ǿ� �¸�
		isEnd = 1;
		strcpy(buf, "GEnd\nMafia\n");
	}

	if(isEnd){
		//���� ���� �ʱ�ȭ
		printf("end");
		game->state = STATE_READY;
		game->start = 1;
		game->mafia_only = 0;
		game->voted = 0;
		game->revote = 0;

		//���Ǿ� ����Ʈ �ۼ�
		for(i = 0; i < game->clnt_num; i++)
			if(game->clnt[i].mafia){
				strcat(buf, game->clnt[i].name);
				if(i != last_mafia)
					strcat(buf, ", ");
			}

		//GEnd �޽��� ����
		while(game->handling_usr1)
			;

		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);

		//���� ���� �ʱ�ȭ
		for(i = 0; i < game->clnt_num; i++){
			game->clnt[i].mafia = 0;
			game->clnt[i].start = 0;
			game->clnt[i].voted = 0;
			game->clnt[i].killed = 0;
		}
		return 1;
	}

	return 0;
}
