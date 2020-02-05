#include "mysh.h"

int main(void) {

  lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
  stream = fdopen(STDIN_FILENO, "r");
  tokens = malloc(sizeof(char *) * MAX_TOKENS);
  pipe_tokens = malloc(sizeof(char *) * MAX_TOKENS);

  do {
    printf("mysh> ");
    read_command();
  } while (run_command() != EXIT_CMD);

  return 0;
}

void read_command() {
  getline(&lineptr, &MAX_LINE_LEN, stream);
  // printf("Shell read this line: %s\n", lineptr);
  tokenize(lineptr);
}

void pipe_tokenize(char *pipe_str_p) {
  int pipe_token_count = 0;
  printf("======PIPES======\n");
  while (*pipe_str_p != '\0') {
    printf("%c", *pipe_str_p);
    if (*pipe_str_p == '|') {
      printf("\n");
    }
    pipe_str_p++;
    pipe_token_count++;
  }
  printf("====END PIPES====\n");
}
void tokenize(char *string) {
  int token_count = 0;
  // int size = MAX_TOKENS;
  char *this_token;
  pipe_tokenize(string);

  while ((this_token = strsep(&string, " \t\v\f\n\r")) != NULL) {
    printf("this_token = %s\n", this_token);
    if (*this_token == '\0')
      continue;

    tokens[token_count] = this_token;
    // printf("Token %d: %s\n", token_count, tokens[token_count]);
    token_count++;
  }
  print_tokens();
}

int run_command() {

  // printf("strcmp(tokens[0], \"exit\") = %d\n", strcmp(tokens[0], "exit"));

  if (strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "x") == 0) {
    return EXIT_CMD;
  }

  pid_t child_pid;
  child_pid = fork();
  if (child_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (child_pid == 0) {
    int exec_return_value = execvp(tokens[0], tokens);
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

void print_tokens() {
  for (int i = 0; i < sizeof(tokens); ++i)
    printf("%s ", tokens[i]);

  printf("\n");
}
void pipe_recursive() {
  int fds[2];
  char buf[30];
  pid_t pid1, pid2, pid;
  int status, i;

  /* create a pipe */
  if (pipe(fds) == -1) {
    perror("pipe");
    exit(1);
  }

  /* fork first child */
  if ((pid1 = fork()) < 0) {
    perror("fork");
    exit(1);
  }

  if (pid1 == 0) {
    close(1);        /* close normal stdout (fd = 1) */
    dup2(fds[1], 1); /* make stdout same as fds[1] */
    close(fds[0]);   /* we don't need the read end -- fds[0] */

    if (execlp("ps", "ps", "-elf", (char *)0) < 0) {
      perror("Child");
      exit(0);
    }

    /* control never reaches here */
  }

  /* fork second child */
  if ((pid2 = fork()) < 0) {
    perror("fork");
    exit(1);
  }

  if (pid2 == 0) {
    close(0);        /* close normal stdin (fd = 0)*/
    dup2(fds[0], 0); /* make stdin same as fds[0] */
    close(fds[1]);   /* we don't need the write end -- fds[1]*/

    if (execlp("less", "less", (char *)0) < 0) {
      perror("Child");
      exit(0);
    }

    /* control never reaches here */
  }

  /* parent doesn't need fds  - MUST close - WHY? */
  close(fds[0]);
  close(fds[1]);

  /* parent waits for children to complete */
  for (i = 0; i < 2; i++) {
    pid = wait(&status);
    printf("Parent: Child %d completed with status %d\n", pid, status);
  }
}