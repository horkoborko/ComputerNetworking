CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -pedantic -pthread -c  $(DEBUG)
LFLAGS = -Wall -std=c99 -pedantic -pthread $(DEBUG)

server : t_server.c threadpool.o
	$(CC) $(LFLAGS) t_server.c threadpool.o -o tserver

threadpool.o : threadpool.c
	$(CC) $(CFLAGS) threadpool.c



clean:
	\rm *.o client
