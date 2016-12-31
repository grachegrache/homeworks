/*
 * mysort.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#ifndef MYSORT_H_
#define MYSORT_H_

#include <fcntl.h>
#include <string.h>

struct id{
  char ch;
  struct id* next;
};
typedef struct id str;

struct rec{
  str* id;
  struct rec* next;
};
typedef struct rec record;

extern void print_man(const char*);

int init_file(char*, record*);
void merge_sort(record*, int);
void rewrite(char*, record*);
record* search(record*, int);
int compare(record*, record*);
void free_rec(record*, int);
void free_str(str*);

#endif /* MYSORT_H_ */
