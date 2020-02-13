#include "mypipe.h"
#include "myheader.h"

int main() {
  printf("hello");
  mypipe();
}

void pipe_tokenize(char *pipe_str_p)
{
  int pipe_token_count = 0;
  printf("======PIPES======\n");
  while (*pipe_str_p != '\0')
  {

    printf("%c", *pipe_str_p);
    if (*pipe_str_p == '|')
    {
      printf("\n");
    }
    pipe_str_p++;
    pipe_token_count++;
  }
  printf("====END PIPES====\n");
}
void mypipe() {

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