# à la http://nuclear.mutantstargoat.com/articles/make/

src = $(wildcard *.c)
hdr = $(wildcard *.h)
obj = $(src:.c=.o)
# dep = $(obj:.o=.d)

CC = gcc
LDFLAGS = -lm
DBGFLAGS = -Wall -Wextra -g

mysh: $(obj) $(hdr)
	$(CC) -o $@ $^ $(LDFLAGS) $(DBGFLAGS)


.PHONY: run clean gdb mem

gdb: mysh
	gdb mysh

mem: mysh
	valgrind --leak-check=full ./mysh

run: mysh
	./mysh

clean:
	rm -f $(obj) mysh
