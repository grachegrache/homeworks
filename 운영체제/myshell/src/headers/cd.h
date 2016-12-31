/*
 * cd.h
 *
 *  Created on: 2015. 11. 3.
 *      Author: panda
 */

#ifndef CD_H_
#define CD_H_

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdlib.h>

extern char full_path[];
extern char path[];
extern void print_man(const char*);
void set_path();
char* get_homedir();

#endif /* CD_H_ */
