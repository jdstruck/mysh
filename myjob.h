#ifndef _MYJOB_H
#define _MYJOB_H
#include "myheader.h"

// process & job structs from GNU manual "Implementing a Job Control Shell"
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
} job;

struct job *create_job();
struct process *create_process(char **);
void add_process_to_job(job *, process *);
void print_job_queue(struct job *);

#endif