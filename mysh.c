#include "mysh.h"

size_t len = MAX_LINE_LEN;
FILE *stream;
char *lineptr = NULL;
char **tokens;

int main(void) {
  assert((stream = fdopen(STDIN_FILENO, "r")) != NULL);
  do {
    printf("mysh> ");
    read_command();
  } while (run_command() != 0);
  return 0;
}

int run_command() {

  printf("lineptr: %s\n", lineptr);
  printf("strcmp(lineptr, \"exit\") = %d\n", strcmp(lineptr, "exit"));
  if (strcmp(lineptr, "exit")) {
    printf("exiting\n");
    return 1;
  }

  return 1;
}

void read_command() {
  getline(&lineptr, &len, stream);
  tokenize(lineptr);
}

void tokenize(char *string) { printf("Shell read this line: %s\n", lineptr); }
