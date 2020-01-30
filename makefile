test: mysh
	./mysh
	ps -l

mysh: mysh.c
	gcc -g -Wall -o mysh mysh.c

clean:
	-rm mysh