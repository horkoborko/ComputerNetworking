CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -pedantic -c $(DEBUG)
LFLAGS = -Wall -std=c99 -pedantic $(DEBUG)

client : client.c
	$(CC) $(LFLAGS) client.c -o client

server: server.c server.h
	$(CC) $(LFLAGS) -lpthread server.c -o server


clean:
	rm /*.o
