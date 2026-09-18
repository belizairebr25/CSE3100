CC=gcc
CFLAGS=-g -Wall -std=c99 #Wall means warn all
Targets=ex-factorial catalan

all: $(TARGETS)

catalan: catalan.c
	$(CC) $(CFLAGS) -o catalan catalan.c

ex-factorial: ex-factorial.c
	$(CC) $(CFLAGS) -o ex-factorial ex-factorial.c

clean: 
	rm -rf *.0 *~ $(TARGETS) a.out

