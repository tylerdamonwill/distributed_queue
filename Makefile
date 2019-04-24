CC := clang 
CFLAGS := -g

all: server client test input

clean:
	rm -rf server client test server.dSYM client.dSYM

server: server.c socket.h
	$(CC) $(CFLAGS) -o server server.c -lpthread

client: client.c
	$(CC) $(CFLAGS) -o client client.c -lpthread

test: test.c
	$(CC) $(CFLAGS) -o test test.c

input: inputTest.c
	$(CC) $(CFLAGS) -o input inputTest.c
