#ifndef _MYSH_H_
#define _MYSH_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_TOKENS 100
#define MAX_STRING_LEN 1024

size_t const MAX_LINE_LEN = 10000;

#define EXIT_STR "exit"
#define EXIT_CMD 0
#define UNKNOWN_CMD 99

#endif
