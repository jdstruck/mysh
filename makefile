test: mysh
	./mysh
	ps -l

mysh: mysh.c
	gcc -g -Wall -Werror -o mysh mysh.c

clean:
	-rm mysh