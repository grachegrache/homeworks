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
#define TIME_CHAT 10
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
	char* type;
	char* contents;
} message;

typedef struct {
	char name[10];
	int start;
	int voted;
	int killed;
	int mafia;
	int sock;
} user;

typedef struct {
	int clnt_num;
	int clnt_scrt_num;
	char* state;
	user clnt[CLIENT_MAX];
	user clnt_scrt[CLIENT_MAX];
	int mafia_only;
	int start;
	int voted;
	int revote;
	int handling_usr1;
	int handling_usr2;
} game_info;

void initialize_mmap();
void error_handling(char *message);
void read_childproc(int sig);
void handler_usr1(int sig);
void handler_usr2(int sig);
void handler_arlam(int sig);
game_info* reset_game();
void reset_clnt();
int get_index(char* username);
game_info* get_mmap();
void remove_clnt(int index);
void remove_scrt_clnt(int index);
message* parse_message(char* buf);
char* make_message(message* msg, int index);
int check_gameover();
#endif /* SERVER_H_ */
