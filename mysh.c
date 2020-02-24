#include "myheader.h" 
#include "mysh.h"
#include <time.h>
// #include "mypipe.h"
void print_char_ptr_arr(char **char_array_p) {
  for (size_t i = 0; i < sizeof(char_array_p); ++i)
    printf("%s ", char_array_p[i]);
}
struct job *first_job = NULL;
// setbuf(stdout, 0);

void prompt(){
	// We print the prompt in the form "<user>@<host> <cwd> >"
	char hostn[1204] = "";
	gethostname(hostn, sizeof(hostn));
	printf("%s@%s > ", getenv("LOGNAME"), hostn/*, getcwd(cwd, 1024)*/);
}

int main(void) {
    lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
    stream = fdopen(STDIN_FILENO, "r");
    // argv = malloc(sizeof(char *) * MAX_TOKENS);
    pipe_tokens = malloc(sizeof(char *) * MAX_TOKENS);

    do {
        prompt();
        read_command();
    } while (process_jobs() != EXIT_CMD);
    return 0;
}

void launch_process(process *p, int infile, int outfile, int errfile) {
    if(infile != STDIN_FILENO) {
        dup2(infile, STDIN_FILENO);
        close(infile); 
    }
    if(outfile != STDOUT_FILENO) {
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }
    if(errfile != STDERR_FILENO) {
        dup2(errfile, STDERR_FILENO);
        close(errfile);
    }
    fflush(stdout);
    if(execvp(p->argv[0], p->argv) == -1) {
        perror("exevp");
        exit(1);
    }
}

int launch_job(job *j){
    pid_t pid;
    int mypipe[2], infile, outfile;
    infile = j->stdin;
    process *p = j->first_process;
    // run_command(p->argv);
    if (strcmp(p->argv[0], "exit") == 0 ||
        strcmp(p->argv[0], "x") == 0 || 
        strcmp(p->argv[0], "q") == 0) {
        return EXIT_CMD;
    }

    // Call built-in change directory
    if (strcmp(p->argv[0], "cd") == 0) return cd(p->argv);
    int process_counter = 0;
    for(; p; p = p->next) {
        if (p->next) {
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
            launch_process(p, infile, outfile, j->stderr);
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
    
    fflush(stdout);
    for(int i = 0; i < process_counter; ++i) {
        int wait_status;
        if (waitpid(-1,&wait_status,WUNTRACED) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }
    first_job = NULL;
    return 1;
}
void read_command() {
    getline(&lineptr, &MAX_LINE_LEN, stream);
}

void pipe_tokenize(char *pipe_str_p) {
    char *pipe_token;
    while ((pipe_token = strsep(&pipe_str_p, "|")) != NULL) {
        if(*pipe_token == '\0')
            continue;

        char **argv = arg_tokenize(pipe_token);
        process *p = create_process(argv);
        // TODO: fix add_process_to_job to update job in place
        // TODO: add job->next after each return
        
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
int cd(char **argv) {
	if (argv[1] == NULL) {
		chdir(getenv("HOME")); 
		return 1;
	} else { 
		if (chdir(argv[1]) == -1) {
			printf(" %s: no such directory\n", argv[1]);
            return -1;
		}
	}
	return 1;
}
int process_jobs() {
    char **argv;

    // If command (i.e. lineptr) empty, return immediately
    if(strlen(lineptr) <= 1) {
        return 1;
    }
    // run_command(arg_tokenize(lineptr));
    // If pipes detected
    // if(pipe_count(lineptr) >= 0) {
    pipe_tokenize(lineptr);
    return launch_job(first_job);
    // } else {
        // return run_command(arg_tokenize(lineptr));
    // } 
  
    // return 1;
}

// int run_command(char **argv) {
//     if (strcmp(argv[0], "exit") == 0 ||
//         strcmp(argv[0], "x") == 0 || 
//         strcmp(argv[0], "q") == 0) {
//       return EXIT_CMD;
//     }

//     // Call built-in change directory
//     if (strcmp(argv[0], "cd") == 0) return cd(argv);

//     // // Run single job (no pipes)
//     // pid_t child_pid;
//     // child_pid = fork();
//     // if (child_pid == -1) {
//     //     perror("fork");
//     //     exit(EXIT_FAILURE);
//     // }

//     // if (child_pid == 0) {
//     //     if (execvp(argv[0], argv) == -1) {
//     //         perror("execvp");
//     //         exit(EXIT_FAILURE);
//     //     }
//     // } else {
//     //     int wait_status;
//     //     pid_t terminated_child_pid = wait(&wait_status);
//     //     if (terminated_child_pid == -1) {
//     //         perror("wait");
//     //         exit(EXIT_FAILURE);
//     //     } 
//     // }
//     return 1;
// }

int pipe_count(char *argv) {
    int count = 0;
    for(int i = 0; argv[i]; ++i) {
       if(argv[i] == '|') ++count;
    }
    return count;
}
struct job *create_job() {
    struct job *j = malloc(sizeof(struct job));
    j->next = NULL;
    j->first_process = NULL;
    j->tail = NULL;
    j->stdin = STDIN_FILENO;
    j->stdout = STDOUT_FILENO;
    j->stderr = STDERR_FILENO;
    j->p_count = 0;
    return j;
}

struct process *create_process(char **argv) {
    struct process *p = malloc(sizeof(struct process));
    p->next = NULL;
    p->argv = argv;
    return p;
}
void add_process_to_job(job *j, process *p) {
    if(j==NULL) {
        j = create_job();
        j->first_process = j->tail = p;
    } else if (j->first_process == j->tail) {
        j->tail = j->first_process->next = p;
    } else {
        j->tail->next = p;
        j->tail = p;
    }

    // print_char_ptr_arr(p->argv);
    // print_job_queue(j);
}

void print_job_queue(job *j_arg) {
    for (job *j = j_arg; j; j = j->next) {
        int c = 0;
        for (process *p = j->first_process; p; p = p->next) {
            print_char_ptr_arr(p->argv);
            ++c;
        }
        printf("%d process(es)\n",c);
    }
}

