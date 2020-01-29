#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  // print prompt (>) to terminal
  // read command from terminal
  // pid=fork()
  // if(pid==0) {
  //	rc=exec(command)
  //	if(rc!=0) {print error; continue;}
  //}
  // cmdrc=waitpid(pid)

  // duplicate pathname in exec, i.e. execlp(pathname, arg0, .. argn) is there
  // for aliasing

  // READ ABOUT Virtual Memory PAGING
  // Why 4K pages??

  pid_t child_pid;

  // Create child process
  child_pid = fork();

  // Check if it was successful to create child process
  if (child_pid == -1) {
    perror("fork unsuccessful");
    exit(EXIT_FAILURE);
  }

  // Parent is waiting for the child to complete its task
  if (child_pid == 0) {
    printf("### Child ###\nCurrent PID: %d and Child PID: %d\n", getpid(),
           child_pid);
    int exec_return_value = execlp("ls", "ls", (char *)NULL);
    if (exec_return_value == -1) {
      perror("execlp failed");
      exit(EXIT_FAILURE);
    }
  } else {
    printf("### Parent ###\nCurrent PID: %d and Child PID: %d\n", getpid(),
           child_pid);
    int wait_status;
    pid_t terminated_child_pid = wait(&wait_status);
    if (terminated_child_pid == -1) {
      perror("wait failure");
      exit(EXIT_FAILURE);
    } else
      printf("Parent: my child %d terminates.\n", terminated_child_pid);
  }

  exit(EXIT_SUCCESS);
}
