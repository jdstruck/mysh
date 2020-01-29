#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	pid_t child_pid;

	// Create child process
	child_pid = fork();

	// Check if it was successful to create child process
	if (child_pid == -1)
	{
		perror("fork unsuccessful");
		exit(EXIT_FAILURE);
	}

	// Parent is waiting for the child to complete its task
	if (child_pid == 0)
	{
		int exec_return_value = execlp("/bin/ls", "ls", "-l", (char *)NULL);
		if (exec_return_value == -1)
		{
			perror("execlp failed");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int wait_status;
		pid_t terminated_child_pid = wait(&wait_status);
		if (terminated_child_pid == -1)
		{
			perror("wait failure");
			exit(EXIT_FAILURE);
		}
		else
			printf("Parent: my child %d terminates.\n", terminated_child_pid);
	}

	exit(EXIT_SUCCESS);
}
