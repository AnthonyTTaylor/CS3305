CC      = gcc
CFLAGS  = -g -O0  -std=gnu11 -lz -lpthread
LDFLAGS = -L.

.PHONY : all test clean

all: client3 server3

client3: client3.c
	$(CC) $(CFLAGS) -o $@ -c $<

server3: server3.c
		$(CC) $(CFLAGS) -o $@ -c $< -l rt
clean:
	rm -f *.o *.a client3 server3
