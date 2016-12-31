/*
 * main.h
 *
 *  Created on: 2015. 10. 27.
 *      Author: panda
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
//#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inner_command.h"

//define
#define MAX_BUF 10000
#define MAX_PATH 1000

//pid_t shell_pgid;
//struct termios shell_tmodes;
//int shell_terminal;
//int shell_is_interactive;

typedef struct{
	char* command;
	char* param;
} command;

enum operator{
	END, AMPERSAND, PIPE, REDIR_IN, REDIR_OUT
};
//buffer
unsigned int buffer_point;
char buffer[MAX_BUF];

//path
char full_path[MAX_PATH];
char path[MAX_PATH];

extern void set_path();
//extern char* get_homedir();

int call_function(command*);
int execute_shellscript(char*);
void init_shell();
bool match_op(enum operator*, enum operator);
int parse_arg(char*, char**);
int parse_command(char*, command*);
void parse_param(command*, char*[]);
void print_prompt();
void print_man(const char*);
int process_command(char*, enum operator*, int[]);
int process_line();
int process_char(char);
void split_command(int, int, int, char**);
void run_shell();

#endif /* MAIN_H_ */
