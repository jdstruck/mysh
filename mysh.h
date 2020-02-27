#ifndef _MYSH_H_
#define _MYSH_H_
#include "myheader.h"

#define MAX_TOKENS 1024
#define MAX_STRING_LEN 1024
#define EXIT_STR "exit"
#define EXIT_CMD 0
#define RETURN_TO_PROMPT 1
#define UNKNOWN_CMD 99
#define ANOTHER 100

size_t MAX_LINE_LEN = 10000;
FILE *stream;
char *lineptr = NULL;
char **pipe_tokens;

// Process & job structs and job control procedures adapted from
// GNU manual "Implementing a Job Control Shell"
// https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html
typedef struct process {
    struct process *next; /* next process in pipeline */
    char **argv;          /* command line args for exec */
    pid_t pid;            /* process id */
    char completed;       /* true if process has completed */
    char stopped;         /* true if process has stopped */
    int status;           /* reported status value */
} process;

typedef struct job {
    struct job *next;          /* next active job */
    char *command;             /* command line, used for messages */
    process *first_process;    /* list of process in this job */
    process *tail;             /* last process in job (tail==first_process if only one) */
    pid_t pgid;                /* process group ID */
    char notified;             /* true if user told about stopped job */
    int stdin, stdout, stderr; /* saved i/o channels */
    int p_count;
} job;

struct job *create_job();
struct process *create_process(char **);
void add_process_to_job(job *, process *);
void print_char_ptr_arr(char **);
void print_job_queue(struct job *);
int pipe_count(char *);

void pipe_tokenize(char *);
void pipe_recursive();
char **arg_tokenize(char *);
void read_command();
int run_command(char **);
int process_jobs();
void launch_process(process *, int, int, int);
int launch_job(job *);
int cd(char **);
int count_a_char(char *, char);
int find_str_in_argv(char **, char *);
void cleanup();
int check_for_keywords(char **);
int arg_count(char **);
#endif
