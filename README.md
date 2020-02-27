# My toy shell for CS 550

## Instructions
Enter `make run` to compile and run the shell

## Notes
### Pipes
Pipes should work as expected for an arbitrary number of commands in the pipeline. 

### Redirection
*Simple* input and output redirection is supported.

#### Output redirection
Output redirection works if the `>` is placed before a filename after a single command -- `ls > out.txt` -- or if placed at the end of a pipeline -- `ls | grep text > out.txt`. Output redirection will not work for a command on the left side, or middle, of a pipe.

#### Input redirection
Input redirection is supported if the `<` is placed before a filename after a single command -- `grep text < text.txt` -- or in the first command of a pipeline -- `grep text < text.txt | wc -l`. Input redirection will not work in the middle or at the end of a pipeline. 

#### Combine input and output redirection
Input and output redirection can be combined in the same command under certain conditions. In a single command, i.e. without pipes, the command can read from a file, then output to another file -- `grep text < text.txt > grep_text.txt`. In a pipeline, the first command can read from a file, and the last command can output to a file -- `grep text < text.txt | wc -l > wc_text.txt` -- however as indicated above, input redirection anywhere but the first command, and output redirection anywhere but the last command, is invalid.

### Indentification of invalid combinations
Some invalid combinations are handled by the system, including more than one input or output redirection in a pipeline, and input or output redirection to the left or in the middle of a pipeline.
