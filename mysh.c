// #include "myheader.h" 
#include "mysh.h"
#include <time.h>
// #include "mypipe.h"
struct job *first_job = NULL;
// setbuf(stdout, 0);

void prompt(){
	char hostn[1204] = "";
	gethostname(hostn, sizeof(hostn));
	printf("%s@%s > ", getenv("LOGNAME"), hostn);
}

int main(void) {
    lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
    stream = fdopen(STDIN_FILENO, "r");
    pipe_tokens = malloc(sizeof(char *) * MAX_TOKENS);

    do {
        prompt();
        read_command();
    } while (process_jobs() != EXIT_CMD);
    return 0;
}

void read_command() {
    getline(&lineptr, &MAX_LINE_LEN, stream);
}

int process_jobs() {

    // If command (i.e. lineptr) empty, return immediately
    if(strlen(lineptr) <= 1) {
        return RETURN_TO_PROMPT;
    }

    // If more than one redirect in either direction, return immediately
    if(count_a_char(lineptr, '>') > 1 || count_a_char(lineptr, '<') > 1) {
        fprintf(stderr, "Invalid operation: too many redirects\n");
        return RETURN_TO_PROMPT;
    }

    // Parse line into job/process structs
    pipe_tokenize(lineptr);

    // Process job
    return launch_job(first_job);
}

int launch_job(job *j){
    pid_t pid;
    int mypipe[2], infile, outfile;
    infile = j->stdin;
    process *p = j->first_process;

    // Check if exit cmd ("exit", "x", or "q") has been entered
    if (strcmp(p->argv[0], "exit") == 0 ||
        strcmp(p->argv[0], "x") == 0 || 
        strcmp(p->argv[0], "q") == 0) {
        return EXIT_CMD;
    }

    // Check if built-in change directory has been envoked with "cd"
    if (strcmp(p->argv[0], "cd") == 0) return cd(p->argv);

    // Otherwise process jobs
    for(; p; p = p->next) {
        if (p->next) {
            if(find_str_in_argv(p->argv, ">") > 0){
                fprintf(stderr, "Invalid command: a command on the left side of a pipe cannot use output redirection.\n");
                cleanup();
                return RETURN_TO_PROMPT;
            }
            if (pipe(mypipe) <0) {
                perror("pipe");
                exit(1);
            }
            outfile = mypipe[1];
        } else {

            // Check if last command redirects output to file
            int strpos = find_str_in_argv(p->argv, ">");
            if(strpos > 0){
                char *outfilename = p->argv[strpos+1];
                char *args_aux[MAX_TOKENS];
                p->argv[strpos] = NULL;
                int redirfile;

                // Process redirection
                if((redirfile = open(outfilename, O_CREAT|O_TRUNC|O_WRONLY, 0600)) < 0) {
                    perror(outfilename);
                    exit(1);
                } 
                outfile = redirfile;
            } else {

                // Otherwise output to stdout
                outfile = j->stdout;
            }
        }
        
        pid = fork();
        if (pid <0) {

            // Fork failed
            perror("fork");
            exit(1);

        } else if(pid == 0) {

            // This is the child processs
            launch_process(p, infile, outfile, j->stderr);

        } else {

            // This is the parent process
            p->pid = pid;

        }

        // Close file descriptors as appropriate
        if(infile != j->stdin)
            close(infile);
        if(outfile != j->stdout)
            close(outfile);
        infile = mypipe[0];
    }
    
    fflush(stdout);

    // Wait for child processes to finish
    for(int i = 0; i < j->p_count; ++i) {
        int wait_status;
        if (waitpid(-1,&wait_status,WUNTRACED) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }
    cleanup();
    return RETURN_TO_PROMPT;
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
    if(execvp(p->argv[0], p->argv) == -1) {
        perror("exevp");
        exit(1);
    }
}

void cleanup() {
    first_job = NULL;
}

void pipe_tokenize(char *pipe_str_p) {
    char *pipe_token;
    while ((pipe_token = strsep(&pipe_str_p, "|")) != NULL) {
        if(*pipe_token == '\0')
            continue;

        char **argv = arg_tokenize(pipe_token);
        process *p = create_process(argv);
        
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
    j->tail->next = NULL;
    j->p_count++;
}
struct process *create_process(char **argv) {
    struct process *p = malloc(sizeof(struct process));
    p->next = NULL;
    p->argv = argv;
    return p;
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

int cd(char **argv) {
	if (argv[1] == NULL) {
		chdir(getenv("HOME")); 
		return RETURN_TO_PROMPT;
	} else { 
		if (chdir(argv[1]) == -1) {
			printf(" %s: no such directory\n", argv[1]);
            return -1;
		}
	}
	return RETURN_TO_PROMPT;
}

void print_char_ptr_arr(char **argv) {
    for (size_t i = 0; i < sizeof(argv); ++i)
        printf("%s ", argv[i]);
    printf("\n");
}

int arg_count(char **argv) {
    size_t i;
    for (i = 0; argv[i] != NULL; ++i){}
    return i;

}

int find_str_in_argv(char **argv, char *s) {
    for (size_t i = 0; argv[i] != NULL; ++i) {
        if(strcmp(argv[i], s) == 0) {
            return i;
        }
    }
    return -1;
}

int pipe_count(char *argv) {
    int count = 0;
    for(int i = 0; argv[i]; ++i) {
       if(argv[i] == '|') ++count;
    }
    return count;
}

int count_a_char(char *argv, char c) {
    int count = 0;
    for(int i = 0; argv[i]; ++i) {
       if(argv[i] == c) ++count;
    }
    return count;
}