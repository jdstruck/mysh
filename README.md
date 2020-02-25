# My toy shell for CS 550

## Instructions

Enter `make run` to compile and run the shell

## Notes

### Pipes

Pipes should work as expected for an arbitrary number of commands in the pipeline. 

### Redirection

*Simple* input and output redirection is supported.

Output redirection works if the `>` is placed before a filename after a single command -- `ls > out.txt` -- or if placed at the end of a pipeline -- `ls | grep text > out.txt`.

Input redirection will not work in a pipeline, but will work with a single command, and in conjunction with output redirection if applicable. 

Neither input or output redirection will work for a command on the left side, or middle, of a pipe. 

### Indentification of invalid combinations

Some invalid combinations are handled by the system, including more than one input or output redirection in a pipeline, and input or output redirection to the left or in the middle of a pipeline.
