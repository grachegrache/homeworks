/*
 * mymkdir.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#ifndef MYMKDIR_H_
#define MYMKDIR_H_

#include <errno.h>

#define MAX_PATH 1000

extern char full_path[];

extern char* get_homedir(void);
extern void print_man(const char*);

char* create_directories(char*);
char* get_absolute_path(char*);

#endif /* MYMKDIR_H_ */
