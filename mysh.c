#include "mysh.h"

int main(void) {
  FILE *stream;
  char *lineptr = NULL;
  size_t len = MAX_LINE_LEN;

  assert((stream = fdopen(STDIN_FILENO, "r")) != NULL);
  while (1) {
    printf("jsh> ");
    getline(&lineptr, &len, stream);
    printf("Shell read this line: %s\n", lineptr);

    if (*lineptr == 'x' || strcmp(lineptr, "exit")) {
      printf("exiting\n");
      exit(0);
    }
  }
  return 0;
}
