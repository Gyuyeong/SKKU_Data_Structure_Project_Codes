CC=gcc
CFLAGS=-O3 -Wall

all: linked_list blocked_linked_list

linked_list: linked_list.c
	$(CC) -o $@ $< $(CFLAGS)

blocked_linked_list: blocked_linked_list.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f linked_list blocked_linked_list
