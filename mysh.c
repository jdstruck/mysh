#include "mysh.h"

size_t len = MAX_LINE_LEN;
FILE *stream;
char *lineptr = NULL;
char **tokens;

int main(void) {
  assert((stream = fdopen(STDIN_FILENO, "r")) != NULL);
  while (1) {
    printf("mysh> ");
    read_command();
  }
  return 0;
}

int run_command() {

  if (strcmp(lineptr, "x") || strcmp(lineptr, "exit")) {
    printf("exiting\n");
    exit(EXIT_SUCCESS);
  }
  return 0;
}

void read_command() {
  getline(&lineptr, &len, stream);
  tokenize(lineptr);
  run_command();
}

void tokenize(char *string) { printf("Shell read this line: %s\n", lineptr); }
