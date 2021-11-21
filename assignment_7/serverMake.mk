CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -pedantic -pthread -c $(DEBUG)
LFLAGS = -Wall -std=c99 -pedantic -pthread $(DEBUG)

server : computeserver.c
	$(CC) $(LFLAGS) computeserver.c -o server -lm


clean:
	\rm *.o server
