#ifndef _MYSH_H_
#define _MYSH_H_
#include "myheader.h"
#include "myjob.h"

#define MAX_TOKENS 1024
#define MAX_STRING_LEN 1024
#define EXIT_STR "exit"
#define EXIT_CMD 0
#define UNKNOWN_CMD 99
#define ANOTHER 100

size_t MAX_LINE_LEN = 10000;
FILE *stream;
char *lineptr = NULL;
char **pipe_tokens;

void pipe_tokenize(char *);
void pipe_recursive();
char **arg_tokenize(char *);
void read_command();
int run_command(char **);
int process_jobs();
void launch_process(process *, int, int, int);
int launch_job(job *);

#endif
