#ifndef CMDLINE_H
void initCmdLine(); // Required one time before running getCmdLine
char ** getCmdLine(); // Returns a list of white-space delimited tokens read from stdin
		// Last element in the list is a pointer to NULL
		// Results from previous call are invalidated by a new call to getCmdLine
void freeCmdLine(); // Required one time before exiting

#define CMDLINE_H
#endif