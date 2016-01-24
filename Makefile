CC      = gcc
CFLAGS  = -g -O0  -std=gnu11 -lz
LDFLAGS = -L.

.PHONY : all test clean

all: shell

shell.o: shell.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o *.a shell
