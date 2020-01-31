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
#define EXIT_STR "exit"
#define EXIT_CMD 0
#define UNKNOWN_CMD 99
size_t MAX_LINE_LEN = 10000;
FILE *stream;
char *lineptr = NULL;
char **tokens;

void tokenize(char *);
void print_tokens();
void read_command();
int run_command();

#endif
