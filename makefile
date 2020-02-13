src = $(wildcard *.c)
obj = $(src:.c=.o)

CFLAGS = -Wall -Wextra

LDFLAGS = -lGL -lglut -lpng -lz -lm

myprog: $(obj)
	(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog