/*
 * hex.h
 *
 *  Created on: 2015. 5. 3.
 *      Author: panda
 */

#ifndef HEX_H_
#define HEX_H_

#include <stdio.h>

char get_hex(int r);
int get_dec(char h);
char* dectohex(int n);
char* get_complement_of_hex(char* hex, int size);
char* get_formated_hex(char* hex, int size);



#endif /* HEX_H_ */
