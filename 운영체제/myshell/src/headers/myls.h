/*
 * myls.h
 *
 *  Created on: 2015. 11. 3.
 *      Author: panda
 */

#ifndef MYLS_H_
#define MYLS_H_

#include <dirent.h>
//#include <unistd.h>
#include <sys/stat.h>
//#include <sys/types.h>
#include <stdbool.h>

extern char full_path[];
extern void print_man(char*);

char get_dir_type(unsigned char);
void get_stat(char* name, struct stat*);
void print_file(struct dirent*);

#endif /* MYLS_H_ */
