#include "mysh.h"

int main(void) {

  lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
  stream = fdopen(STDIN_FILENO, "r");
  tokens = malloc(sizeof(char *) * MAX_TOKENS);

  do {
    printf("mysh> ");
    read_command();
  } while (run_command() != EXIT_CMD);

  return 0;
}

int run_command() {

  printf("lineptr: %s\n", lineptr);
  printf("strcmp(tokens[0], \"exit\") = %d\n", strcmp(tokens[0], "exit"));

  if (strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "x") == 0) {
    return EXIT_CMD;
  }

  return UNKNOWN_CMD;
}

void read_command() {
  getline(&lineptr, &MAX_LINE_LEN, stream);
  printf("Shell read this line: %s\n", lineptr);
  tokenize(lineptr);
}

void tokenize(char *string) {
  int token_count = 0;
  // int size = MAX_TOKENS;
  char *this_token;
  while ((this_token = strsep(&string, " \t\v\f\n\r")) != NULL) {

    if (*this_token == '\0')
      continue;

    tokens[token_count] = this_token;
    printf("Token %d: %s\n", token_count, tokens[token_count]);
    token_count++;
  }
}
