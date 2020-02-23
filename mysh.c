#include "myheader.h" 
#include "mysh.h"
#include "mypipe.h"


struct job *first_job = NULL;

int main(void) {
    lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
    stream = fdopen(STDIN_FILENO, "r");
    // argv = malloc(sizeof(char *) * MAX_TOKENS);
    pipe_tokens = malloc(sizeof(char *) * MAX_TOKENS);

    // while(1) {
    //     printf("mysh> ");
    //     read_command();
    //     launch_job(first_job,1);
    // }
    do {
      printf("mysh> ");
      read_command();
    } while (process_jobs() != EXIT_CMD);
    return 0;
}

void launch_process(process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground) {
    //pid_t pid = getpid();
    //if (pgid == 0) pgid = pid;
    //setpgid (pid, pgid);

    if(infile != STDIN_FILENO) {
        dup2(infile, STDIN_FILENO);
        close(infile); 
        printf("launch_process: infile != STDIN_FILENO\n");
    }
    if (outfile != STDOUT_FILENO) {
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
        printf("launch_process: infile != STDOUT_FILENO\n");
    }
    if(errfile != STDERR_FILENO) {
        printf("launch_process: infile != STDERR_FILENO\n");
        dup2(errfile, STDERR_FILENO);
        close(errfile);
    }https://github.com/jdstruck/simple-c-shell/blob/master/simple-c-shell.c
    //printf("end of launch_process");
    execvp(p->argv[0], p->argv);
    perror("execvp");
    exit(1);
}

int launch_job(job *j, int foreground){
    pid_t pid;
    int mypipe[2], infile, outfile;
    if(j->first_process) {
        printf("printf job queue\n");
        print_job_queue(first_job);
        printf("end printf job queue\n");
    } else printf("no processes");

    infile = j->stdin;
    process *p = j->first_process;
    int process_counter = 0;
    for(; p; p = p->next) {
        printf("process %d\n", process_counter+1);
        // print_char_ptr_arr(p->argv);
        if (p->next) {
            printf("p->next exists\n");
            if (pipe(mypipe) <0) {
                perror("pipe");
                exit(1);
            }
            outfile = mypipe[1];
        } else {
            outfile = j->stdout;
        }
        
        pid = fork();
        if(pid == 0) {
            launch_process(p, j->pgid, infile, outfile, j->stderr, foreground);
        } else if (pid <0) {
            perror("fork");
            exit(1);
        } else {
            p->pid = pid;
        }
        if(infile != j->stdin)
            close(infile);
        if(outfile != j->stdout)
            close(outfile);
        infile = mypipe[0];
        ++process_counter;
    }
    for(int i = 0; i < pipe_count(lineptr)+1; ++i) {
        int wait_status;
        pid_t terminated_child_pid = wait(&wait_status);
        if (terminated_child_pid == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        } else {
            printf("\nParent: my child %d terminates.\n", terminated_child_pid);
        }
    }
    first_job = NULL;
    printf("end of launch job\n");
    return 1;
}
void read_command() {
    getline(&lineptr, &MAX_LINE_LEN, stream);

}

void pipe_tokenize(char *pipe_str_p) {
    char *pipe_token;
    // int pipe_token_count = 0;
    // printf("======PIPE TOKENIZE======\n");
    while ((pipe_token = strsep(&pipe_str_p, "|")) != NULL) {
        if(*pipe_token == '\0')
            continue;

        char **argv = arg_tokenize(pipe_token);
        //print_char_ptr_arr(argv);
        process *p = create_process(argv);
        // TODO: fix add_process_to_job to update job in place
        // TODO: add job->next after each return
        
        //add_process_to_job(first_job, p); 
        if(first_job==NULL) {
            first_job = create_job();
            first_job->first_process = first_job->tail = p;
        } else if (first_job->first_process == first_job->tail) {
            first_job->tail = first_job->first_process->next = p;
        } else {
            first_job->tail->next = p;
            first_job->tail = p;
        }
        first_job->tail->next = NULL;
        ++first_job->p_count;
    }
    // printf("====END PIPE TOKENIZE====\n");
}



char **arg_tokenize(char *string) {
    int arg_token_count = 0;
    char *arg_token;
    char **argv = malloc(sizeof(char *) * MAX_TOKENS);

    while ((arg_token = strsep(&string, " \t\v\f\n\r")) != NULL) {
        if (*arg_token == '\0')
            continue;

        argv[arg_token_count] = arg_token;
        arg_token_count++;
    }
    argv[arg_token_count] = NULL;
    return argv;
}

int process_jobs() {
    // If lineptr empty, return immediately
    if(strlen(lineptr) <= 1) {
        return 1;
    }
    printf("pipe count: %d\n", pipe_count(lineptr));
    // If pipes detected
    if(pipe_count(lineptr) > 0) {
        pipe_tokenize(lineptr);
        return launch_job(first_job, 1);
    } else {
        return run_command(arg_tokenize(lineptr));
    } 
   // printf("string length argv[0]: %d\n", str
    // printf("process exists %d\n", first_job->first_process!=NULL); 
    // if(strcmp(argv[0], "")){
    //     if (strcmp(argv[0], "exit") == 0 ||
    //         strcmp(argv[0], "x") == 0 || 
    //         strcmp(argv[0], "q") == 0) {
    //         return EXIT_CMD;
    //     }
    //     launch_job(first_job, 1);
    // } else {
    //     first_job = NULL;
    // }
    return 1;
}

int pipe_count(char *argv) {
    int count = 0;
    for(int i = 0; argv[i] != NULL; ++i) {
       if(argv[i] == '|') ++count;
    }
    return count;
    
}

int run_command(char **argv) {
    if (strcmp(argv[0], "exit") == 0 ||
        strcmp(argv[0], "x") == 0 || 
        strcmp(argv[0], "q") == 0) {
      return EXIT_CMD;
    }
    // launch_job(first_job,1);

    pid_t child_pid;
    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        int exec_return_value = execvp(argv[0], argv);
        if (exec_return_value == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        int wait_status;
        pid_t terminated_child_pid = wait(&wait_status);
        if (terminated_child_pid == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        } else {
            printf("\nParent: my child %d terminates.\n", terminated_child_pid);
        }
    }
    printf("end of run_command\n");
    return UNKNOWN_CMD;
}

// void
// runsource(int pfd[])	/* run the first part of the pipeline, cmd1 */
// {
// 	int pid;	/* we don't use the process ID here, but you may wnat to print it for debugging */

// 	switch (pid = fork()) {

// 	case 0: /* child */
// 		dup2(pfd[1], 1);	/* this end of the pipe becomes the standard output */
// 		close(pfd[0]); 		/* this process don't need the other end */
// 		execvp(cmd1[0], cmd1);	/* run the command */
// 		perror(cmd1[0]);	/* it failed! */

// 	default: /* parent does nothing */
// 		break;

// 	case -1:
// 		perror("fork");
// 		exit(1);
// 	}
// }

// void
// rundest(int pfd[])	/* run the second part of the pipeline, cmd2 */
// {
// 	int pid;

// 	switch (pid = fork()) {

// 	case 0: /* child */
// 		dup2(pfd[0], 0);	/* this end of the pipe becomes the standard input */
// 		close(pfd[1]);		/* this process doesn't need the other end */
// 		execvp(cmd2[0], cmd2);	/* run the command */
// 		perror(cmd2[0]);	/* it failed! */

// 	default: /* parent does nothing */
// 		break;

// 	case -1:
// 		perror("fork");
// 		exit(1);
// 	}
// }