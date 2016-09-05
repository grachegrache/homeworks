/*
 * server.h
 *
 *  Created on: 2015. 6. 4.
 *      Author: panda
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pwd.h>
#include <time.h>

#define BUF_SIZE 10000
#define CLIENT_MIN 4
#define CLIENT_MAX 8
#define TIME_CHAT 100
#define TIME_VOTE 30

#define STATE_READY "Ready"
#define STATE_DAY "Day"
#define STATE_VOTE "Vote"
#define STATE_NIGHT "Night"

#define TYPE_ACCESS "Access"
#define TYPE_START "Start"
#define TYPE_CHAT "Chat"
#define TYPE_CHOICE "Choice"
#define TYPE_INVITE "Invite"
#define TYPE_EXIT "Exit"

typedef struct {
	char* type;		//�޽��� Ÿ��
	char* contents;	//�޽��� ����
} message;

typedef struct {
	char name[10];//�̸�
	int start;		//���� ����
	int voted;		//��ǥ ����
	int killed;	//���� ����
	int mafia;		//���Ǿ� ����
	int sock;		//���� fd
	int pid;		//���μ��� pid
} user;

typedef struct {
	int clnt_num;					//���� ä�ù� Ŭ���̾�Ʈ ��
	int clnt_scrt_num;			//��� ä�ù� Ŭ���̾�Ʈ ��
	char state[10];				//���� ����
	user clnt[CLIENT_MAX];		//���� ä�ù� Ŭ���̾�Ʈ ���� �迭
	user clnt_scrt[CLIENT_MAX];	//��� ä�ù� Ŭ���̾�Ʈ ���� �迭
	int mafia_only;				//�� ä�ö� ���ǾƵ鸸 ä���ϱ� ���� ����
	int start;						//���� ���۽� ����鿡�� �ù�, ���ǾƸ� �˷��ֱ� ���� ����
	int voted;						//��ǥ �Ϸ� Ȯ���� ���� ����
	int revote;					//����ǥ Ȯ���� ���� ����
	int gend;						//GEnd �޽����� ���� �������� Ȯ���ϱ� ���� ����
	int handling_usr1;			//handler_usr1 ����ȭ�� ���� ����
	int handling_usr2;			//handler_usr2 ����ȭ�� ���� ����
//	int removing_main;
//	int removing_scrt;
} game_info;

void initialize_mmap();
void error_handling(char *message);
void read_childproc(int sig);
void handler_usr1(int sig);
void handler_usr2(int sig);
void handler_arlam(int sig);
game_info* reset_game();
void reset_clnt();
void reset_clnt_scrt();
int get_name_index(char* username, int type);
game_info* get_mmap();
int get_available(int type);
void remove_clnt(int index);
void remove_clnt_scrt(int index);
message* parse_message(char* buf);
char* make_message(message* msg, int index);
int check_gameover(int index);
#endif /* SERVER_H_ */

