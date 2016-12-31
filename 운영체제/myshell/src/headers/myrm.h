/*
 * myrm.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#ifndef MYRM_H_
#define MYRM_H_

#include <errno.h>
#include <dirent.h>

#define MAX_PATH 1000

extern int parse_arg(char*, char**);
extern void print_man(const char*);

void remove_directories(char*);

#endif /* MYRM_H_ */
