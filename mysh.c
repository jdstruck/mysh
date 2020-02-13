#include "myheader.h" 
#include "mysh.h"
#include "mypipe.h"

int main(void)
{
  lineptr = malloc(sizeof(char) * MAX_STRING_LEN);
  stream = fdopen(STDIN_FILENO, "r");
  tokens = malloc(sizeof(char *) * MAX_TOKENS);
  pipe_tokens = malloc(sizeof(char *) * MAX_TOKENS);

  do
  {
    printf("mysh> ");
    read_command();
  } while (run_command() != EXIT_CMD);

  return 0;
}

void read_command()
{
  getline(&lineptr, &MAX_LINE_LEN, stream);
  // printf("Shell read this line: %s\n", lineptr);
  tokenize(lineptr);
}

void tokenize(char *string)
{
  int token_count = 0;
  // int size = MAX_TOKENS;
  char *this_token;
  pipe_tokenize(string);

  while ((this_token = strsep(&string, " \t\v\f\n\r")) != NULL)
  {
    printf("this_token = %s\n", this_token);
    if (*this_token == '\0')
      continue;

    tokens[token_count] = this_token;
    // printf("Token %d: %s\n", token_count, tokens[token_count]);
    token_count++;
  }
  print_tokens();
}

int run_command()
{

  // printf("strcmp(tokens[0], \"exit\") = %d\n", strcmp(tokens[0], "exit"));

  if (strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "x") == 0)
  {
    return EXIT_CMD;
  }

  pid_t child_pid;
  child_pid = fork();
  if (child_pid == -1)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (child_pid == 0)
  {
    int exec_return_value = execvp(tokens[0], tokens);
    if (exec_return_value == -1)
    {
      perror("execvp");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    int wait_status;
    pid_t terminated_child_pid = wait(&wait_status);
    if (terminated_child_pid == -1)
    {
      perror("wait");
      exit(EXIT_FAILURE);
    }
    else
    {
      printf("\nParent: my child %d terminates.\n", terminated_child_pid);
    }
  }

  return UNKNOWN_CMD;
}

void print_tokens()
{
  for (int i = 0; i < sizeof(tokens); ++i)
    printf("%s ", tokens[i]);

  printf("\n");
}