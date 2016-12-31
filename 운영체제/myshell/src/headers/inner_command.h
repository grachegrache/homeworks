/*
 * myshell.h
 *
 *  Created on: 2015. 10. 27.
 *      Author: panda
 */

#ifndef INNER_COMMAND_H_
#define INNER_COMMAND_H_

void myexit(char*);
void mycd(char*);
void mypwd(char*);
void myenv(char*);
void myls(char*);
void mycat(char*);
void myhead(char*);
void mytail(char*);
void mysort(char*);
void mycmp(char*);
void mycp(char*);
void mymv(char*);
void mymkdir(char*);
void myrmdir(char*);
void myrm(char*);
void myps(char*);

const static struct {
  const char *name;
  void (*func)(char*);
} function_map [] = {
  {"exit", myexit},
  {"cd", mycd},
  {"mypwd", mypwd},
  {"myenv", myenv},
  {"myls", myls},
  {"mycat", mycat},
  {"myhead", myhead},
  {"mytail", mytail},
  {"mysort", mysort},
  {"mycmp", mycmp},
  {"mycp", mycp},
  {"mymv", mymv},
  {"mymkdir", mymkdir},
  {"myrmdir", myrmdir},
  {"myrm", myrm},
  {"myps", myps}
};

#endif /* INNER_COMMAND_H_ */
