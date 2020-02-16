#include "myheader.h" 
#include "mysh.h"
#include "mypipe.h"

int main(void) {
  lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
  stream = fdopen(STDIN_FILENO, "r");
  command_tokens = malloc(sizeof(char *) * MAX_TOKENS);
  pipe_tokens = malloc(sizeof(char *) * MAX_TOKENS);

  do {
    printf("mysh> ");
    read_command();
  } while (run_command() != EXIT_CMD);

  return 0;
}

void read_command() {
  getline(&lineptr, &MAX_LINE_LEN, stream);
  arg_tokenize(lineptr);
}

void pipe_tokenize(char *pipe_str_p) {
  char *pipe_token;
  int pipe_token_count = 0;
  printf("======PIPES======\n");
  while ((pipe_token = strsep(&pipe_str_p, "|")) != NULL) {
    printf("pipe_token = %s\n", pipe_token);
    if(*pipe_token == '\0')
      continue;

    add_process_to_job(first_job, arg_tokenize(pipe_token)); 

    pipe_tokens[pipe_token_count] = pipe_token;
    pipe_token_count++;
  }
  print_char_ptr_arr(pipe_tokens);
  printf("====END PIPE TOKENIZE====\n");
}

void add_process_to_job(job *j, char **args) {
  if(j == NULL) {
    j = malloc(sizeof(job));
    j->first_process = NULL;
  }
  process *p;
  for(p = j->first_process; p; p = p->next) {
    if(p == NULL) {
      p = malloc(sizeof(process));
      p->argv = args;
    }
  }
}

char **arg_tokenize(char *string) {
  int arg_token_count = 0;
  char *arg_token;
  pipe_tokenize(string);

  while ((arg_token = strsep(&string, " \t\v\f\n\r")) != NULL) {
    printf("this_token = %s\n", arg_token);
    if (*arg_token == '\0')
      continue;

    command_tokens[arg_token_count] = arg_token;
    arg_token_count++;
  }
  print_char_ptr_arr(command_tokens);
  return command_tokens;
}

int run_command() {
  if (strcmp(command_tokens[0], "exit") == 0 ||
      strcmp(command_tokens[0], "x") == 0 || 
      strcmp(command_tokens[0], "q") == 0) {
    return EXIT_CMD;
  }

  pid_t child_pid;
  child_pid = fork();
  if (child_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (child_pid == 0) {
    int exec_return_value = execvp(command_tokens[0], command_tokens);
    if (exec_return_value == -1) {
      perror("execvp");
      exit(EXIT_FAILURE);
    }
  }
  else {
    int wait_status;
    pid_t terminated_child_pid = wait(&wait_status);
    if (terminated_child_pid == -1) {
      perror("wait");
      exit(EXIT_FAILURE);
    }
    else {
      printf("\nParent: my child %d terminates.\n", terminated_child_pid);
    }
  }

  return UNKNOWN_CMD;
}