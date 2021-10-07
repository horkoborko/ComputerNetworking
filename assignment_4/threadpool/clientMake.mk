CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -pedantic -pthread -c  $(DEBUG)
LFLAGS = -Wall -std=c99 -pedantic -pthread $(DEBUG)

client : threadpool_client.c threadpool.o
	$(CC) $(LFLAGS) threadpool_client.c threadpool.o -o client

threadpool.o : threadpool.c
	$(CC) $(CFLAGS) threadpool.c

server : multiThreadServer.c
	$(CC) $(LFLAGS) -lpthread multiThreadServer.c -o server

clean:
	rm /*.o
