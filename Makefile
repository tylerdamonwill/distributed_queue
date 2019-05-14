CC := clang 
CFLAGS := -g -fsanitize=address

all: server client

clean:
	rm -rf server client server.dSYM client.dSYM

server: server.c socket.h
	$(CC) $(CFLAGS) -o server server.c -lpthread

client: client.c
	$(CC) $(CFLAGS) -o client client.c -lpthread
