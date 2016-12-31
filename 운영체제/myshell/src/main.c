/*
 * main.c
 *
 *  Created on: 2015. 10. 27.
 *      Author: panda
 */

/* Keep track of attributes of the shell.  */

#include "headers/main.h"

int main() {
	init_shell();
	run_shell();
}

int call_function(command* line) {
	int i;
	for(i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++)
		if(!strcmp(function_map[i].name, line->command)
				&& function_map[i].func){
			function_map[i].func(line->param);
			return 0;
		}

	return -1;
}

int execute_shellscript(char* filename){
	int fd, i;
	i=0;
	fd = open(filename, O_RDONLY);
	char buf[1];
	if(fd == -1){
		printf("error:can not open the file!\n");
		return -1;
	}

	while(read(fd, buf, 1) > 0)
		if(buf[0] == '\n')
			break;
	char sh[lseek(fd,0,SEEK_CUR)];
	lseek(fd,2,SEEK_SET);
	while(read(fd, buf, 1) > 0)
		if(buf[0] == '\n')
			break;
		else
			sh[i++] = buf[0];
	sh[i] = 0;
	close(fd);

	command* cmd = (command*) malloc(sizeof(command));
	cmd->command = sh;
	cmd->param = filename;
	char** params = (char**) malloc(sizeof(char*));
	parse_param(cmd, params);

	pid_t pid = fork();
	if(pid == 0){
		//child process
		execv(cmd->command, params);
		exit(0);

	}else
		//parent process
		waitpid(pid, 0, 0);

	i=0;
	while(params[i] != 0)
		free(params[i++]);
	free(params);
	free(cmd);

	return 0;
}

void init_shell() {
	/* Ignore interactive and job-control signals.  */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

//init buffer
	memset(buffer, 0, MAX_BUF);
	buffer_point = 0;

	//init path
	memset(full_path, 0, MAX_PATH);
	memset(path, 0, MAX_PATH);
	getcwd(full_path, MAX_PATH);
	set_path();

	//init env path
	strcpy(buffer, "PATH=");
	strcpy(buffer, strcat(buffer, getenv("PATH")));
	strcpy(buffer, strcat(buffer, ":./"));
	putenv(strdup(buffer));
	memset(buffer, 0, MAX_BUF);
	char* PATH = getenv("PATH");

	printf("full_path: %s\npath: %s\nPATH: %s\n", full_path, path, PATH);
}

bool match_op(enum operator* op, enum operator match) {
	int i = 0;
	while(op[i] != END)
		if(op[i++] == match)
			return true;
	return false;
}

//parse arg -> parsed
//ignore braces between parameters
int parse_arg(char* arg, char** parsed) {
	int i, quotation, start_point, arg_count;
	quotation = 0;
	start_point = 0;
	arg_count = 0;

	if(arg == NULL || strlen(arg) == 0)
		return 0;

	for(i = 0; i < strlen(arg); i++){
		switch(arg[i]){
			case '\"':
				quotation++;
				break;
			case ' ':
				if(quotation % 2 == 0){
					if(start_point != -1){
						//parse arg
						parsed[arg_count++] = strndup(arg + start_point,
								i - start_point);
						start_point = -1;
					}
				}
				break;
			default:
				if(start_point == -1)
					start_point = i;
				break;
		}
	}

	parsed[arg_count++] = strndup(arg + start_point, i - start_point);

	return arg_count;
}

//parse str -> command
int parse_command(char* str, command* com) {
	int i, command_point, param_point, command_len;
	command_point = -1;
	param_point = -1;
	command_len = -1;

	if(str == NULL)
		return 1;

	//trim right side of the str
	for(i = strlen(str); i >= 0; i++){
		if(str[i] == ' ')
			str[i] = 0;
		else
			break;
	}

	if(strlen(str) == 0){
		free(str);
		return 1;
	}

	//ignore braces
	for(i = 0; i < strlen(str); i++){
		switch(str[i]){
			case ' ':
				if(command_point != -1)
					if(command_len == -1)
						command_len = i - command_point;
				break;
			default:
				if(command_point == -1)
					command_point = i;
				else if(command_len != -1)
					if(param_point == -1)
						param_point = i;
				break;
		}

		if(param_point != -1)
			break;
	}

	if(command_len == -1){
		command_len = i - command_point;
		param_point = i;
	}

	com->command = strndup(str + command_point, command_len);
	if(param_point < strlen(str))
		com->param = strndup(str + param_point, strlen(str) - param_point);
	else
		com->param = strdup("");

	return 0;
}

//parse command -> argv for execute execv or execvp
void parse_param(command* line, char* argv[]) {
	int count = 0;
	argv[count++] = strdup(line->command);

	count += parse_arg(line->param, argv + 1);
	argv[count] = NULL;
}

void print_man(const char* command) {
	if(strcmp(command, "exit") == 0){
		printf("exit shell\n");
		printf("exit");

	}else if(strcmp(command, "cd") == 0){
		printf("change directory\n");
		printf("cd [FILENAME]");

	}else if(strcmp(command, "mypwd") == 0){
		printf("print working directory\n");
		printf("mypwd");

	}else if(strcmp(command, "myenv") == 0){
		printf("print environment [STRING]\n");
		printf("if [STRING] is null, print all\n");
		printf("myenv\n");
		printf("myenv [STRING]");

	}else if(strcmp(command, "myls") == 0){
		printf("list directory contents\n");
		printf("myls [FILENAME]");

	}else if(strcmp(command, "mycat") == 0){
		printf("concatenate\n");
		printf("print [NUMBER] bytes\n");
		printf("if [NUMBER] is null, print all\n");
		printf("mycat [FILENAME]\n");
		printf("mycat [FILENAME] [NUMBER]\n");
		printf("[NUMBER] is positive interger");

	}else if(strcmp(command, "myhead") == 0){
		printf("print [NUMBER] lines from start\n");
		printf("if [NUMBER] is null, print 5 lines\n");
		printf("myhead [FILENAME]\n");
		printf("myhead [FILENAME] [NUMBER]\n");
		printf("[NUMBER] is positive integer");

	}else if(strcmp(command, "mytail") == 0){
		printf("print [NUMBER] lines from end\n");
		printf("if [NUMBER] is null, print 5 lines\n");
		printf("myhead [FILENAME]\n");
		printf("myhead [FILENAME] [NUMBER]\n");
		printf("[NUMBER] is positive integer");

	}else if(strcmp(command, "mysort") == 0){
		printf("sort file line by line as ascending order\n");
		printf("mysort [FILENAME]");

	}else if(strcmp(command, "mycmp") == 0){
		printf("compare two files\n");
		printf("mycmp [FILENAME] [FILENAME]");

	}else if(strcmp(command, "mycp") == 0){
		printf("copy [SOURCE] file to [DEST] file\n");
		printf("mycp [OPTION] [SOURCE] [DEST]\n");
		printf("[OPTION]\n");
		printf("-f: force to write");

	}else if(strcmp(command, "mymv") == 0){
		printf("move [SOURCE] file to [DEST] file\n");
		printf("mymv [OPTION] [SOURCE] [DEST]\n");
		printf("[OPTION]\n");
		printf("-f: force to write");

	}else if(strcmp(command, "mymkdir") == 0){
		printf("make directory\n");
		printf("mymkdir [OPTION] [DIRECTORY]\n");
		printf("[OPTION]\n");
		printf("-p: make parents if it does not exists");

	}else if(strcmp(command, "myrmdir") == 0){
		printf("remove directory\n");
		printf("myrmdir [DIRECTORY]");

	}else if(strcmp(command, "myrm") == 0){
		printf("remove\n");
		printf("myrm [FILENAME]\n");
		printf("myrm [OPTION] [FILENAME]\n");
		printf("[OPTION]\n");
		printf("-rf: recursive force to remove");

	}else if(strcmp(command, "myps") == 0){
		printf("report a snapshot of the current running processes\n");
		printf("myps");

	}

	printf("\n");
}

void print_prompt() {
	printf("[20132385 %s]$", path);
}

int process_command(char* str, enum operator* op, int fd[]) {
	int i, res;
	command* line = (command*) malloc(sizeof(command));
//	i = 0;
	res = 1;

	if(parse_command(str, line))
		return 0;

	if(call_function(line) == -1){
		//not inner command
		pid_t pid = fork();

		if(pid == 0){
			//child
			char** params = (char**) malloc(sizeof(char*));
			parse_param(line, params);

			if(match_op(op, REDIR_IN)){
				if(match_op(op, PIPE))
					close(fd[1]);
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
			}
			if(match_op(op, REDIR_OUT)){
				if(match_op(op, PIPE))
					close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}

			if(line->command[0]
					== '/'|| strstr(line->command, "./") != NULL || strstr(line->command, "../") != NULL || strstr(line->command, "~/") != NULL)
				//path
				execv(line->command, (char* const *) params);

			else
				//search external command from PATH
				execvp(line->command, (char* const *) params);

			//execv or execvp failed
			//check .sh
			if(strstr(str, ".sh") != NULL)
				execute_shellscript(str);
			else
				printf("error:command not found!\n");

//			i = 0;
//			while(params[i] != NULL)
//				free(params[i++]);
//			free(params);

			exit(0);

		}else if(pid > 0){
			//parent
			if(!(match_op(op, AMPERSAND) || match_op(op, PIPE)))
				waitpid(pid, 0, 0);
			else
				res = pid;

			if(match_op(op, REDIR_IN))
				close(fd[0]);
			if(match_op(op, REDIR_OUT))
				close(fd[1]);
		}
	}

	free(str);
	free(line->command);
	free(line->param);
	free(line);
	return res;
}

int process_line() {
	int i, j, quotation, command_point, command_count, op_count, res;
	int** fds = (int**) malloc(sizeof(int*));
	pid_t* pids = (pid_t*) malloc(sizeof(pid_t));
	char** commands = (char**) malloc(sizeof(char*));
	enum operator** op = (enum operator**) malloc(sizeof(enum operator*));
	res = 0;
	quotation = 0;
	command_point = 0;
	command_count = 0;
	op_count = 0;
	op[command_count] = (enum operator*) malloc(sizeof(enum operator));
	commands[command_count] = NULL;
	fds[command_count] = NULL;

	for(i = 0; i < buffer_point; i++){
		switch(buffer[i]){
			case '\"':
				quotation++;
				break;
			case '&':
				if(quotation % 2 == 0){
					//operator &
					op[command_count][op_count++] = AMPERSAND;
					op[command_count][op_count] = END;

					if(op_count == 1){
						//first operator
						split_command(i, command_count, command_point,
								commands);
					}

					//find next command's start point
					//ignore braces
					j = i;
					while(j < buffer_point && buffer[++j] == ' ')
						;
					command_point = j;
					i = j - 1;
					op_count = 0;
					op[++command_count] = (enum operator*) malloc(
							sizeof(enum operator));
					commands[command_count] = NULL;
					fds[command_count] = NULL;
				}
				break;
			case '|':
				if(quotation % 2 == 0){
					//pipe
					op[command_count][op_count++] = PIPE;
					op[command_count][op_count++] = REDIR_OUT;
					op[command_count][op_count] = END;

					if(fds[command_count] == NULL)
						fds[command_count] = (int*) calloc(2, sizeof(int));

					if(op_count == 2){
						//first operator
						split_command(i, command_count, command_point,
								commands);
					}
					int fd[2];
					pipe(fd);
					fds[command_count][0] = fd[0];
					fds[command_count][1] = fd[1];

					//find next command's start point
					//ignore braces
					j = i;
					while(j < buffer_point && buffer[++j] == ' ')
						;
					command_point = j;
					i = j - 1;
					op_count = 0;
					op[++command_count] = (enum operator*) malloc(
							sizeof(enum operator));

					op[command_count][op_count++] = PIPE;
					op[command_count][op_count++] = REDIR_IN;
					fds[command_count] = (int*) calloc(2, sizeof(int));
					fds[command_count][0] = fd[0];
					fds[command_count][1] = fd[1];
				}
				break;
			case '<':
				if(quotation % 2 == 0){
					//redirect stdin
					op[command_count][op_count++] = REDIR_IN;

					if(fds[command_count] == NULL)
						fds[command_count] = (int*) calloc(2, sizeof(int));

					if(op_count == 1){
						//first operator
						split_command(i, command_count, command_point,
								commands);
					}

					//ignore braces
					j = i;
					while(buffer[++j] == ' ')
						;
					int start_point = j;
					//find next brace
					while(j < buffer_point && buffer[++j] != ' ')
						;
					i = j - 1;
					char* filename = strndup(buffer + start_point,
							j - start_point);
					fds[command_count][0] = open(filename, O_RDONLY);
					free(filename);
					if(fds[command_count][0] == -1){
						printf("error:can not open the file!\n");
						res = -1;
					}
				}
				break;
			case '>':
				if(quotation % 2 == 0){
					//redirect stdout
					op[command_count][op_count++] = REDIR_OUT;

					if(fds[command_count] == NULL)
						fds[command_count] = (int*) calloc(2, sizeof(int));

					if(op_count == 1){
						//first operator
						split_command(i, command_count, command_point,
								commands);
					}
					//ignore braces
					j = i;
					while(buffer[++j] == ' ')
						;
					int start_point = j;
					//find next brace
					while(j < buffer_point && buffer[++j] != ' ')
						;
					i = j - 1;
					char* filename = strndup(buffer + start_point,
							j - start_point);
					fds[command_count][1] = open(filename,
					O_CREAT | O_RDWR | O_TRUNC, 0644);
					free(filename);
					if(fds[command_count][1] == -1){
						printf("error:can not open the file!\n");
						res = -1;
					}
				}
				break;
		}
	}

	op[command_count][op_count] = END;
	if(commands[command_count] == NULL){
		//ignore braces
		j = buffer_point;
		while(buffer[--j] == ' ');
		commands[command_count] = strndup(buffer + command_point,
				j - command_point + 1);
	}
	command_count++;

	//execute commands
	for(i = 0; i < command_count; i++)
		pids[i] = process_command(commands[i], op[i], fds[i]);

	//wait child and free op, fds, commands
	for(i = 0; i < command_count; i++){
		if(match_op(op[i], AMPERSAND) || match_op(op[i], PIPE))
			waitpid(pids[i], 0, 0);
		if(match_op(op[i], AMPERSAND))
			printf("[%d] terminated\n", pids[i]);

//		free(commands[i]);
		free(op[i]);
		if(fds[i] != NULL)
			free(fds[i]);
	}

	free(pids);
	free(op);
	free(fds);
	free(commands);
	memset(buffer, 0, MAX_BUF);
	buffer_point = 0;

	return res;
}

int process_char(char ch) {
	bool is_tab, is_enter;
	is_tab = false;
	is_enter = false;

	switch(ch){
		case '\t':
			is_tab = true;
			break;
		case '\b':
			if(buffer_point > 0)
				buffer[--buffer_point] = 0;
			break;
		case '\n':
			is_enter = true;
			if(process_line())
				return -1;
			break;
		default:
			if(buffer_point < MAX_BUF)
				buffer[buffer_point++] = ch;
	}

	return is_enter;
}

//split command from command_point ~ i
//and adjust parameters value
void split_command(int i, int command_count, int command_point, char** commands) {
	int j;
	//ignore braces
	j = i;
	while(buffer[--j] == ' ');
	commands[command_count] = strndup(buffer + command_point,
			j - command_point + 1);
}

void run_shell() {
	char input;

	while(true){
		print_prompt();

		while(true){
			//char input
			input = getc(stdin);
			//process
			if(process_char(input))
				break;
		}
	}
}
