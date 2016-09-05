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
int log_fd = -1;

//SHARE MEMORY 영역
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

	//log 파일 열기
	log_fd = fopen("log.txt", "w+");
	if(log_fd == -1)
		return -1;
	
	//game_info 초기화
	initialize_mmap();
	game = (game_info*) malloc(sizeof(game_info));
	game = reset_game();
	if(game == NULL)
		return -1;

	if(argc >= 2)
		main_port = argv[1];
	if(argc == 3)
		secret_port = argv[2];

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sig_state = sigaction(SIGCHLD, &act, 0);

	//main server 소켓 만들기
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(main_port));

	//secret server 소켓 만들기
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

	//pipe 생성 및 game->clnt, game->clnt_scrt 초기화
	pipe(pipe_serv);
	pipe(pipe_scrt);
	reset_clnt();
	reset_clnt_scrt();

	//시그널 등록
	signal(SIGALRM, handler_arlam);
	signal(SIGUSR1, handler_usr1);
	signal(SIGUSR2, handler_usr2);

	pid = fork();
	if(pid == -1)
		return -1;

	if(pid == 0){
		//비밀 채팅방 서버
//		game = get_mmap();
		close(serv_sock);

		while(true){
			struct sockaddr clnt_adr;
			adr_sz = sizeof(clnt_adr);
			int actual = accept(secret_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
			int available = get_available(0);
			if(available == -1){
				char message[] = "Error\nFull of Clients!\n";
				write(game->clnt_scrt[available].sock, message, sizeof(message));
				close(game->clnt_scrt[available].sock);
				continue;
			}
			game->clnt_scrt[available].sock = actual;

			fprintf(log_fd, "actual sock: %d\n", actual);
			fprintf(log_fd, "비밀 채팅방 sock: %d\n", game->clnt_scrt[available].sock);

			if(game->clnt_scrt[available].sock == -1)
				continue;
			else
				fprintf(log_fd, "new secret client connected...\n");

			pid = fork();
			if(pid == -1){
				close(game->clnt_scrt[available].sock);
				continue;
			}

			if(pid == 0){
				//자식 프로세스
				close(secret_sock);
				game->clnt_scrt_num++;
//				game = get_mmap();
				game->clnt_scrt[available].pid = getpid();
				int index = available;
				message* msg;

				fprintf(log_fd, "비밀 채팅방 클라이언트 %d, sock %d\n", index, game->clnt_scrt[index].sock);

				while(true){
					str_len = read(game->clnt_scrt[index].sock, buf, BUF_SIZE);
					if(str_len == 0)
						break;

					fprintf(log_fd, "비밀 채팅방 요청 %d, sock %d\n%s\n", index,
							game->clnt_scrt[index].sock, buf);

					if(str_len > 0){
						//Start, Choice 메시지 무시
						if(strstr(buf, TYPE_START) != NULL)
							continue;
						else if(strstr(buf, TYPE_CHOICE) != NULL)
							continue;

						else if((msg = parse_message(buf)) == NULL){
							char message[] = "Error\nWrong Message!\n";
							write(game->clnt_scrt[index].sock, message, sizeof(message));

						}else{
							//메시지 만들기
							if(strcmp(msg->type, TYPE_CHAT) == 0){
								strcpy(send_buf, "Chatted\n");
								strcat(send_buf, game->clnt_scrt[index].name);
								strcat(send_buf, "\n");
								strcat(send_buf, msg->contents);

							}else if(strcmp(msg->type, TYPE_INVITE) == 0){
								if(get_name_index(msg->contents, 0) == -1){
									strcpy(send_buf, "Invited\n");
									strcat(send_buf, msg->contents);
								}else
									strcpy(send_buf, "Error\nAlready invited");

							}else if(strcmp(msg->type, TYPE_ACCESS) == 0){
								strcpy(game->clnt_scrt[index].name, msg->contents);
								strcpy(send_buf, "Accept\n");
								strcat(send_buf, msg->contents);

							}else if(strcmp(msg->type, TYPE_EXIT) == 0){
								strcpy(send_buf, "Exited\n");
								strcat(send_buf, game->clnt_scrt[index].name);
							}
							strcat(send_buf, "\n");

							//Error 메시지면 해당 클라이언트에게만 보냄
							if(strstr(send_buf, "Error") != NULL)
								write(game->clnt[index].sock, send_buf, sizeof(send_buf));

							else{
								game->handling_usr2 = 1;
								write(pipe_scrt[1], send_buf, sizeof(send_buf));
								kill(getppid(), SIGUSR2);
								while(game->handling_usr2)
									;

								if(strstr(send_buf, "Exited") != NULL)
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
				remove_clnt_scrt(index);
				fprintf(log_fd, "secret client disconnected...\n");
//				munmap(game, sizeof(game));

				return 0;
			}

		}

		close(secret_sock);
//		munmap(game, sizeof(game));
		return 0;

	}else{
		//메인 채팅방 서버
		close(secret_sock);

		while(true){
			struct sockaddr clnt_adr;
			adr_sz = sizeof(clnt_adr);
			int actual = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
			int available = get_available(1);
			if(available == -1){
				char message[] = "Error\nFull of Clients!\n";
				write(game->clnt[available].sock, message, sizeof(message));
				close(game->clnt[available].sock);
				continue;
			}

			game->clnt[available].sock = actual;

			fprintf(log_fd, "actual sock: %d\n", actual);
			fprintf(log_fd, "메인 채팅방 sock: %d\n", game->clnt[available].sock);

			if(game->clnt[available].sock == -1)
				continue;
			else
				fprintf(log_fd, "new client connected...\n");

			pid = fork();
			if(pid == -1){
				close(game->clnt[available].sock);
				continue;
			}

			if(pid == 0){
				//자식 프로세스
//				game = get_mmap();
				close(serv_sock);
				game->clnt_num++;
				game->clnt[available].pid = getpid();
				int index = available;
				message* msg;

				fprintf(log_fd, "메인 채팅방 클라이언트 %d, sock %d\n", index, game->clnt[index].sock);

				while(true){
					str_len = read(game->clnt[index].sock, buf, BUF_SIZE);
					if(str_len == 0)
						break;

					fprintf(log_fd, "메인 채팅방 요청 %d, sock %d\n%s\n", index, game->clnt[index].sock,
							buf);

					if(str_len > 0){
						if((msg = parse_message(buf)) == NULL){
							char message[] = "Error\nWrong Message!\n";
							write(game->clnt[index].sock, message, sizeof(message));

						}else{
							char* tmp = make_message(msg, index);
							if(tmp == NULL || strlen(tmp) == 0)
								continue;

							strcpy(send_buf, tmp);

							//에러 메시지면 해당 클라이언트에게만 보냄
							if(strstr(send_buf, "Error") != NULL)
								write(game->clnt[index].sock, send_buf, sizeof(send_buf));

							else if(strcmp(msg->type, TYPE_ACCESS) == 0){
								int i;
								for(i = 0; i < CLIENT_MAX; i++){
									if(game->clnt[i].pid != 0){
										memset(send_buf, 0, BUF_SIZE);
										strcpy(send_buf, "Accept\n");
										strcat(send_buf, game->clnt[i].name);
										strcat(send_buf, "\n");

										game->handling_usr1 = 1;
										write(pipe_serv[1], send_buf, sizeof(send_buf));
										kill(getppid(), SIGUSR1);
										while(game->handling_usr1)
											;
									}
								}
							}else{
								game->handling_usr1 = 1;
								write(pipe_serv[1], send_buf, sizeof(send_buf));
								kill(getppid(), SIGUSR1);
								while(game->handling_usr1)
									;

								if(strstr(send_buf, "Exited") != NULL){
									break;

								}else if(strstr(send_buf, "Started") != NULL){
									//게임 시작 가능 확인
									if(game->clnt_num >= CLIENT_MIN){
										int total_start = 1;
										int i;
										for(i = 0; i < CLIENT_MAX; i++)
											if(game->clnt[i].pid != 0)
												total_start &= game->clnt[i].start;

										if(total_start){
											//게임 시작
											game->gend = 0;

											//마피아 선정
											time_t current_time;
											time(&current_time);
											srand(current_time);
											int first_mafia = rand() % CLIENT_MAX;
											int second_mafia = rand() % CLIENT_MAX;

											//첫번째 마피아 선정
											while(game->clnt[first_mafia].pid == 0)
												first_mafia = rand() % CLIENT_MAX;
											game->clnt[first_mafia].mafia = 1;

											//두번째 마피아 선정
											if(game->clnt_num > 5){
												while(game->clnt[second_mafia].pid == 0)
													second_mafia = rand() % CLIENT_MAX;
												game->clnt[second_mafia].mafia = 1;
											}

											//게임 시작 카운트다운
											alarm(0);
											alarm(3);
										}
									}

								}else if(strstr(send_buf, "Choosen") != NULL){
									alarm(0);
									if(game->voted){
										fprintf(log_fd, "voted\n");
										//투표 완료
										game->voted = 0;
										if(game->mafia_only){
											game->mafia_only = 0;
											strcpy(game->state, STATE_DAY);
										}else{
											game->mafia_only = 1;
											strcpy(game->state, STATE_NIGHT);
										}

										if(strcmp(game->state, STATE_NIGHT) == 0)
											if(game->clnt_num < 6){
												//마피아 1명이면
												strcpy(game->state, STATE_VOTE);

											}

										if(check_gameover(-1) == 0){
											//게임이 끝나지 않았으면
											fprintf(log_fd, "not over\n");
											memset(send_buf, 0, BUF_SIZE);
											strcpy(send_buf, "State\n");
											strcat(send_buf, game->state);

											game->handling_usr1 = 1;
											write(pipe_serv[1], send_buf, sizeof(send_buf));
											kill(getppid(), SIGUSR1);
											while(game->handling_usr1)
												;
											alarm(TIME_CHAT);
										}

									}else if(game->revote){
										//재투표
										game->revote = 0;
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
				if(check_gameover(index))
					alarm(0);

				fprintf(log_fd, "client disconnected...\n");
//				munmap(game, sizeof(game));
				return 0;
			}

		}

		close(serv_sock);
//		munmap(game, sizeof(game));
		close(m_fd);
		close(log_fd);
		return 0;
	}

}

/*
 * mmap 사용을 위해 파일 초기화 함수
 * 
 */
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
	fprintf(log_fd, "removed proc id: %d \n", pid);
}

/*
 * 메인 채팅방 서버가 클라이언트들에게 메시지를 보내는 함수
 * 
 */
void handler_usr1(int sig){
//	while(game->removing_main)
//		;
	int str_len, i;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	str_len = read(pipe_serv[0], buf, BUF_SIZE);

	if(strstr(buf, "State") != NULL && strstr(buf, STATE_VOTE) != NULL){
		//클라이언트 voted 초기화
		for(i = 0; i < CLIENT_MAX; i++)
			if(game->clnt[i].pid != 0)
				game->clnt[i].voted = 0;
	}

	fprintf(log_fd, "핸들러 파이프\n%s\n", buf);

	if(str_len > 0)
		if(strstr(buf, "Chatted") != NULL){
			if(game->mafia_only){
				for(i = 0; i < CLIENT_MAX; i++)
					if(game->clnt[i].pid != 0 && game->clnt[i].mafia)
						write(game->clnt[i].sock, buf, str_len);
			}else{
				for(i = 0; i < CLIENT_MAX; i++)
					if(game->clnt[i].pid != 0)
						write(game->clnt[i].sock, buf, str_len);
			}

		}else{
			for(i = 0; i < CLIENT_MAX; i++)
				if(game->clnt[i].pid != 0)
					write(game->clnt[i].sock, buf, str_len);
		}

	if(game->start && strstr(buf, STATE_DAY) != NULL){
		//게임 시
		game->start = 0;

		for(i = 0; i < CLIENT_MAX; i++){
			if(game->clnt[i].pid != 0){
				//공지사항 메시지 만들기

				memset(buf, 0, BUF_SIZE);
				strcpy(buf, "Chatted\n서버\n");
				strcat(buf, "공지\n당신은 ");
				if(game->clnt[i].mafia)
					strcat(buf, "마피아");
				else
					strcat(buf, "시민");
				strcat(buf, "입니다.");

				write(game->clnt[i].sock, buf, sizeof(buf));
			}
		}
	}

	game->handling_usr1 = 0;
}

/*
 * 비밀 채팅방 서버가 클라이언트들에게 메시지를 보내는 함수
 * 
 */
void handler_usr2(int sig){
//	while(game->removing_scrt)
//		;
	int str_len, i;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	str_len = read(pipe_scrt[0], buf, BUF_SIZE);

	fprintf(log_fd, "핸들러 비밀 파이프\n%s\n", buf);

	if(str_len > 0)
		for(i = 0; i < CLIENT_MAX; i++)
			if(game->clnt_scrt[i].pid != 0)
				write(game->clnt_scrt[i].sock, buf, str_len);

	game->handling_usr2 = 0;
}

void handler_arlam(int sig){
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	//투표가 끝나거나 게임이 끝났을 때 알람이 멈추지 않는 경우 알람 무시
	if(game->voted || game->revote || game->gend){
		fprintf(log_fd, "alarm cancel\n");
		return;
	}

	if(strcmp(game->state, STATE_READY) == 0){
		//카운트가 끝나면
		strcpy(game->state, STATE_DAY);

		strcpy(buf, "State\n");
		strcat(buf, game->state);

		game->handling_usr1 = 1;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);
		while(game->handling_usr1)
			;

		alarm(0);
		alarm(TIME_CHAT);

	}else if(strcmp(game->state, STATE_DAY) == 0){
		//낮이 끝나면
		strcpy(game->state, STATE_VOTE);

		strcpy(buf, "State\n");
		strcat(buf, game->state);

		game->handling_usr1 = 1;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);
		while(game->handling_usr1)
			;

		alarm(0);
		alarm(TIME_VOTE);

	}else if(strcmp(game->state, STATE_NIGHT) == 0){
		//밤이 끝나면

		strcpy(game->state, STATE_VOTE);

		strcpy(buf, "State\n");
		strcat(buf, game->state);

		game->handling_usr1 = 1;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);
		while(game->handling_usr1)
			;

		alarm(0);
		alarm(TIME_VOTE);

	}else if(strcmp(game->state, STATE_VOTE) == 0){
		//투표가 시간제한으로 끝나면
		if(game->mafia_only){
			//마피아 투표가 끝나면
			game->mafia_only = 0;
			strcpy(game->state, STATE_DAY);
		}else{
			//시민+마피아 투표가 끝나면
			game->mafia_only = 1;
			strcpy(game->state, STATE_NIGHT);
		}

		if(strcmp(game->state, STATE_NIGHT) == 0)
			if(game->clnt_num < 6){
				//마피아가 1명이면
				strcpy(game->state, STATE_VOTE);

			}
		strcpy(buf, "State\n");
		strcat(buf, game->state);

		game->handling_usr1 = 1;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);
		while(game->handling_usr1)
			;

		if(strcmp(game->state, STATE_VOTE) == 0){
			alarm(0);
			alarm(TIME_VOTE);
			return;
		}

		alarm(0);
		alarm(TIME_CHAT);
	}
}

/*
 * 게임 변수 초기화
 * 
 */
game_info* reset_game(){
	game_info * game;

	game = get_mmap();
	game->clnt_num = 0;
	game->clnt_scrt_num = 0;
	strcpy(game->state, STATE_READY);
	game->mafia_only = 0;
	game->start = 1;
	game->voted = 0;
	game->revote = 0;
	game->gend = 0;
	game->handling_usr1 = 0;
	game->handling_usr2 = 0;

//	game->removing_main = 0;
//	game->removing_scrt = 0;
	return game;
}

/*
 * 메인 채팅방 클라이언트 배열 초기화
 * 
 */
void reset_clnt(){
	int i;

	for(i = 0; i < CLIENT_MAX; i++){
		memset(game->clnt[i].name, 0, 10);
		game->clnt[i].sock = 0;
		game->clnt[i].start = 0;
		game->clnt[i].voted = 0;
		game->clnt[i].killed = 0;
		game->clnt[i].mafia = 0;
		game->clnt[i].pid = 0;
	}

	game->clnt_num = 0;

}

/*
 * 비밀 채팅방 클라이언트 배열 초기화
 * 
 */
void reset_clnt_scrt(){
	int i;

	for(i = 0; i < CLIENT_MAX; i++){
		memset(game->clnt_scrt[i].name, 0, 10);
		game->clnt_scrt[i].sock = 0;
		game->clnt_scrt[i].start = 0;
		game->clnt_scrt[i].voted = 0;
		game->clnt_scrt[i].killed = 0;
		game->clnt_scrt[i].mafia = 0;
		game->clnt_scrt[i].pid = 0;
	}

	game->clnt_num = 0;
}

/*
 * 유저 이름으로 클라이언트 배열에서
 * 동일한 이름을 갖는 클라이언트 인덱스를 리턴하는 함수
 * 
 */
int get_name_index(char* username, int type){
	int i;

	if(type){
	for(i = 0; i < CLIENT_MAX; i++)
		if(game->clnt[i].pid != 0)
			if(strcmp(game->clnt[i].name, username) == 0)
				return i;
	}else{
		for(i = 0; i < CLIENT_MAX; i++)
				if(game->clnt_scrt[i].pid != 0)
					if(strcmp(game->clnt_scrt[i].name, username) == 0)
						return i;
	}
	return -1;
}

/*
 * mmap으로 메모리 할당하여 주소를 리턴하는 함수
 * 
 */
game_info* get_mmap(){
	if(m_fd < 0)
		if(0 > (m_fd = open("mmap_file", O_RDWR | O_CREAT))){
			fprintf(log_fd, "버퍼에 접근할 수 없습니다.\n");
			return 0;
		}

	return (game_info*) mmap(0, sizeof(game_info), PROT_WRITE | PROT_READ,
	MAP_SHARED, m_fd, 0);
}

/*
 * user 배열에서 빈 클라이언트의 인덱스를 리턴하는 함수
 * 
 * type 1: 메인 채팅방 클라이언트에 대해
 * type 0: 비밀 채팅방 클라이언트에 대해
 */
int get_available(int type){
	int i;
	if(type){
		for(i = 0; i < CLIENT_MAX; i++)
			if(game->clnt[i].pid == 0)
				return i;
	}else{
		for(i = 0; i < CLIENT_MAX; i++)
			if(game->clnt_scrt[i].pid == 0)
				return i;
	}

	return -1;
}

/*
 * 메인 채팅방 클라이언트 제거
 * 
 */
void remove_clnt(int index){
//	game->removing_main = 1;

	int i = index;
	close(game->clnt[index].sock);

//	for(; i < game->clnt_num - 1; i++){
//		game->clnt[i].sock = game->clnt[i + 1].sock;
//		strcpy(game->clnt[i].name, game->clnt[i + 1].name);
//		game->clnt[i].start = game->clnt[i + 1].start;
//		game->clnt[i].voted = game->clnt[i + 1].voted;
//		game->clnt[i].killed = game->clnt[i + 1].killed;
//		game->clnt[i].mafia = game->clnt[i + 1].mafia;
//		game->clnt[i].pid = game->clnt[i + 1].pid;
//	}
	game->clnt[i].sock = 0;
	memset(game->clnt[i].name, 0, 10);
	game->clnt[i].start = 0;
	game->clnt[i].voted = 0;
	game->clnt[i].killed = 0;
	game->clnt[i].mafia = 0;
	game->clnt[i].pid = 0;

	game->clnt_num--;
//	game->removing_main = 0;
}

/*
 * 비밀 채팅방 클라이언트 제거
 * 
 */
void remove_clnt_scrt(int index){
//	game->removing_scrt = 1;

	int i = index;
	close(game->clnt_scrt[index].sock);

//	for(i = index; i < game->clnt_scrt_num - 1; i++){
//		game->clnt_scrt[i].sock = game->clnt_scrt[i + 1].sock;
//		strcpy(game->clnt_scrt[i].name, game->clnt_scrt[i + 1].name);
//		game->clnt_scrt[i].start = game->clnt_scrt[i + 1].start;
//		game->clnt_scrt[i].voted = game->clnt_scrt[i + 1].voted;
//		game->clnt_scrt[i].killed = game->clnt_scrt[i + 1].killed;
//		game->clnt_scrt[i].mafia = game->clnt_scrt[i + 1].mafia;
//		game->clnt_scrt[i].pid = game->clnt_scrt[i + 1].pid;
//	}

	game->clnt_scrt[i].sock = 0;
	memset(game->clnt_scrt[i].name, 0, 10);
	game->clnt_scrt[i].start = 0;
	game->clnt_scrt[i].voted = 0;
	game->clnt_scrt[i].killed = 0;
	game->clnt_scrt[i].mafia = 0;
	game->clnt_scrt[i].pid = 0;

	game->clnt_scrt_num--;
//	game->removing_scrt = 0;
}

/*
 * 클라이언트로부터의 메시지를 파싱하는 함수
 * 
 */
message* parse_message(char* buf){
	if(buf == NULL || strlen(buf) == 0)
		return NULL;

	char* cpy = strdup(buf);
	message* msg = (message*) malloc(sizeof(message));
	msg->type = NULL;
	msg->contents = NULL;

	char* tok = strtok(cpy, "\n");
	if((strcmp(tok, TYPE_ACCESS) && strcmp(tok, TYPE_START) && strcmp(tok, TYPE_CHAT) && strcmp(tok,
	TYPE_CHOICE) && strcmp(tok, TYPE_EXIT) && strcmp(tok, TYPE_INVITE)) != 0){
		free(cpy);
		free(msg);
		return NULL;
	}

	msg->type = strdup(tok);

	if((strcmp(msg->type, TYPE_START) && strcmp(msg->type, TYPE_EXIT))!= 0)
		if((tok = strtok(NULL, "\n")) != NULL){
			msg->contents = strdup(tok);
			if(msg->contents[strlen(tok) - 1] == '\n')
				msg->contents[strlen(tok) - 1] = 0;
		}

	return msg;
}

/*
 * 클라이언트에 보낼 메시지 작성 함수
 * 
 */
char* make_message(message* msg, int index){
	int i;
	char message[BUF_SIZE];
	memset(message, 0, BUF_SIZE);

	if(strcmp(msg->type, TYPE_ACCESS) == 0){
		if(strcmp(game->state, STATE_READY) == 0){
			int exist = 0;
			//존재하는 이름인지 확인
			for(i = 0; i < CLIENT_MAX; i++)
				if(game->clnt[i].pid != 0)
					if(strlen(game->clnt[i].name) != 0 && i != index)
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
			if(!game->clnt[index].start){
				game->clnt[index].start = 1;

				strcpy(message, "Started\n");
				strcat(message, game->clnt[index].name);

			}else{
				strcpy(message, "Error\nAleady started!");
			}
		}else
			strcpy(message, "Error\nNot a Ready State!");

	}else if(strcmp(msg->type, TYPE_CHAT) == 0){
		if(strcmp(game->state, STATE_DAY) == 0){
			strcpy(message, "Chatted\n");
			strcat(message, game->clnt[index].name);
			strcat(message, "\n");
			strcat(message, msg->contents);

		}else if(strcmp(game->state, STATE_NIGHT) == 0){
			if(game->clnt[index].mafia){
				strcpy(message, "Chatted\n");
				strcat(message, game->clnt[index].name);
				strcat(message, "\n");
				strcat(message, msg->contents);

			}else
				strcpy(message, "Error\nNot a Day State!");

		}else
			strcpy(message, "Error\nNot a Day or Night State!");

	}else if(strcmp(msg->type, TYPE_CHOICE) == 0){
		if(strcmp(game->state, STATE_VOTE) == 0){
			//투표 집계
			int vote_index = get_name_index(msg->contents, 1);
			if(vote_index == -1)
				strcpy(message, "Error\nCan not find user name!");

			else{
				int is_error;
				is_error = 0;
				if(game->mafia_only)
					if(game->clnt[index].mafia == 0){
						//마피아 투표에 시민이 투표하면
						is_error = 1;
						strcpy(message, "Error\nIt's a mafia voting");
					}

				if(game->clnt[index].killed){
					//죽은 사람이 투표하면
					is_error = 1;
					strcpy(message, "Error\nYou're dead");
				}

				if(is_error == 0){
					int total_vote, choosen_index, same, voter;
					total_vote = 0;
					choosen_index = 0;
					same = 0;
					game->clnt[vote_index].voted++;

					//투표자 세기
					if(game->mafia_only){
						voter = 0;
						for(i = 0; i < CLIENT_MAX; i++)
							if(game->clnt[i].pid != 0)
								if(game->clnt[i].mafia && !game->clnt[i].killed)
									voter++;
					}else{
						voter = game->clnt_num;

						for(i = 0; i < CLIENT_MAX; i++)
							if(game->clnt[i].pid != 0)
								if(game->clnt[i].killed)
									voter--;
					}

					//최다 득표자 선정
					for(i = 0; i < CLIENT_MAX; i++){
						if(game->clnt[i].pid != 0){
							total_vote += game->clnt[i].voted;

							if(game->clnt[choosen_index].voted < game->clnt[i].voted)
								choosen_index = i;
						}
					}

					//동률 존재 확인
					for(i = 0; i < CLIENT_MAX; i++)
						if(game->clnt[i].pid != 0)
							if(choosen_index != i)
								if(game->clnt[choosen_index].voted == game->clnt[i].voted){
									same = 1;
									break;
								}

					//전원 투표 완료하면
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

					}
				}
			}

		}else
			strcpy(message, "Error\nNot a Vote State!");

	}else if(strcmp(msg->type, TYPE_EXIT) == 0){
		strcpy(message, "Exited\n");
		strcat(message, game->clnt[index].name);
	}

//	if(strlen(message) != 0)
//		strcat(message, "\n");

	return message;
}

/*
 * 게임이 끝났는지 확인하고 GEnd 메시지를 보내는 함수
 * 
 * return 1: 끝남
 * return 0: 끝나지 않음
 * 
 */
int check_gameover(int index){
	if(game->start)
		return 0;

	int i, count_citizen, count_mafia, last_mafia, is_end;
	char buf[BUF_SIZE];
	char mafia_list[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);
	memset(mafia_list, 0, BUF_SIZE);
	count_citizen = 0;
	count_mafia = 0;
	last_mafia = 0;
	is_end = 0;

	//마지막 마피아 인덱스 구하기
	for(i = 0; i < CLIENT_MAX; i++){
		if(game->clnt[i].pid != 0){
			if(game->clnt[i].mafia)
				last_mafia = i;
		}
	}

	//마피아 리스트 작성
	for(i = 0; i < CLIENT_MAX; i++)
		if(game->clnt[i].pid != 0)
			if(game->clnt[i].mafia){
				strcat(mafia_list, game->clnt[i].name);
				if(i != last_mafia)
					strcat(mafia_list, ", ");
			}

	if(index > 0)
		remove_clnt(index);

	if(game->clnt_num < 4){
		is_end = 1;
		strcpy(buf, "GEnd\n\n");

	}else{
		//살아 있는 마피아, 시민 수 세기
		for(i = 0; i < CLIENT_MAX; i++){
			if(game->clnt[i].pid != 0){
				if(!game->clnt[i].killed){
					if(game->clnt[i].mafia)
						count_mafia++;
					else
						count_citizen++;
				}
			}
		}

		if(count_mafia == 0){
			//시민 승리
			is_end = 1;
			strcpy(buf, "GEnd\nCitizen\n");

		}else if(count_mafia >= count_citizen){
			//마피아 승리
			is_end = 1;
			strcpy(buf, "GEnd\nMafia\n");
		}
	}

	if(is_end){
		fprintf(log_fd, "game end\n");
		//게임 변수 초기화
		strcpy(game->state, STATE_READY);
		game->start = 1;
		game->mafia_only = 0;
		game->voted = 0;
		game->revote = 0;
		game->gend = 1;
//		game->removing_main = 0;
//		game->removing_scrt = 0;

		strcat(buf, mafia_list);

//GEnd 메시지 전송
		game->handling_usr1 = 1;
		write(pipe_serv[1], buf, sizeof(buf));
		kill(getppid(), SIGUSR1);
		while(game->handling_usr1)
			;

		//유저 게임 정보 초기화
		for(i = 0; i < CLIENT_MAX; i++){
			game->clnt[i].mafia = 0;
			game->clnt[i].start = 0;
			game->clnt[i].voted = 0;
			game->clnt[i].killed = 0;
		}
		return 1;
	}

	return 0;
}

