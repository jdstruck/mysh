#include "myheader.h" 
#include "mysh.h"
#include "mypipe.h"


struct job *first_job = NULL;

int main(void) {
    lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
    stream = fdopen(STDIN_FILENO, "r");
    // argv = malloc(sizeof(char *) * MAX_TOKENS);
    pipe_tokens = malloc(sizeof(char *) * MAX_TOKENS);

    do {
      printf("mysh> ");
      read_command();
    } while (process_jobs() != EXIT_CMD);
    return 0;
}

void read_command() {
    getline(&lineptr, &MAX_LINE_LEN, stream);
    pipe_tokenize(lineptr);
    print_job_queue(first_job);
}

void pipe_tokenize(char *pipe_str_p) {
    char *pipe_token;
    // int pipe_token_count = 0;
    printf("======PIPE TOKENIZE======\n");
    while ((pipe_token = strsep(&pipe_str_p, "|")) != NULL) {
        if(*pipe_token == '\0')
            continue;

        char **argv = arg_tokenize(pipe_token);
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
    }
    printf("====END PIPE TOKENIZE====\n");
}


char **arg_tokenize(char *string) {
    int arg_token_count = 0;
    char *arg_token;
    char **argv = malloc(sizeof(char *) * MAX_TOKENS);
    //pipe_tokenize(string);

    while ((arg_token = strsep(&string, " \t\v\f\n\r")) != NULL) {
        // printf("this_token = %s\n", arg_token);
        if (*arg_token == '\0')
        continue;

        argv[arg_token_count] = arg_token;
        arg_token_count++;
    }
  // print_char_ptr_arr(argv);
    return argv;
}

int process_jobs() {

    return 1;
}

int run_command(char **argv) {
    if (strcmp(argv[0], "exit") == 0 ||
        strcmp(argv[0], "x") == 0 || 
        strcmp(argv[0], "q") == 0) {
      return EXIT_CMD;
    }

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

    return UNKNOWN_CMD;
}

