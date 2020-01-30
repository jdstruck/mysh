#ifndef _MYSH_H_
#define _MYSH_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_TOKENS 1024
#define MAX_STRING_LEN 1024
#define MAX_LINE_LEN 10000

void tokenize(char *);
void read_command();
int run_command();

#endif
