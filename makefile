# à la http://nuclear.mutantstargoat.com/articles/make/

src = $(wildcard *.c)
hdr = $(wildcard *.h)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

CC = gcc
LDFLAGS = -lm
CFLAGS = -Wall -Wextra

mysh: $(obj) $(hdr)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

#$(obj) : %.o : %.c
	#$(CC) $(CFLAGS) -o $@ -c $<
	#@$(CC) -M $< > $@

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: run clean cleandep cleanall

run:
	./mysh

clean:
	rm -f $(obj) mysh

cleandep:
	rm -f $(dep)

cleanall:
	rm -f $(obj) $(dep) mysh