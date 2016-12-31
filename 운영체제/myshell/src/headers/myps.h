/*
 * myps.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#ifndef HEADERS_MYPS_H_
#define HEADERS_MYPS_H_

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH 1000

struct str{
	char ch;
	struct str* next;
};
typedef struct str string;

extern void print_man(const char*);

int is_digit(char*);
void free_string(string*);

#endif /* HEADERS_MYPS_H_ */
