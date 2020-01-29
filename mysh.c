#include "mysh.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	pid_t pid;
	int status;
	int ret;

	pid = fork();

	if (pid < 0) {
		perror("fork failed:");
		exit(1);
	}

	if (pid == 0) {
		printf("This is the child\n");
		exit(99);
	}

	if (pid > 0) {
		printf("This is parent. The child is %d\n", pid);

		ret = waitpid(pid, &status, 0);
		if (ret < 0) {
			perror("waitpid failed:");
			exit(2);
		}

		printf("Child exigted with status %d\n", WEXITSTATUS(status));
	}
	return 0;
}
