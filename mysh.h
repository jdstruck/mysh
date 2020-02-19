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
// char **command_tokens;

// // process & job structs from GNU manual "Implementing a Job Control Shell"
// // https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html
// typedef struct process {
//   struct process *next; /* next process in pipeline */
//   char **argv;          /* command line args for exec */
//   pid_t pid;            /* process id */
//   char completed;       /* true if process has completed */
//   char stopped;         /* true if process has stopped */
//   int status;           /* reported status value */
// } process;

// typedef struct job {
//   struct job *next;          /* next active job */
//   char *command;             /* command line, used for messages */
//   process *first_process;    /* list of process in this job */
//   pid_t pgid;                /* process group ID */
//   char notified;             /* true if user told about stopped job */
//   int stdin, stdout, stderr; /* saved i/o channels */
// } job;

char **pipe_tokens;

void pipe_tokenize(char *);
void pipe_recursive();
char **arg_tokenize(char *);
void read_command();
int run_command(char **);
int process_jobs();

#endif
