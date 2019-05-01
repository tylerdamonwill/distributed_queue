CC := clang 
CFLAGS := -g

all: server client tester

clean:
	rm -rf server client tester server.dSYM client.dSYM

server: server.c socket.h
	$(CC) $(CFLAGS) -o server server.c -lpthread

client: client.c
	$(CC) $(CFLAGS) -o client client.c -lpthread

tester: tester.c
	$(CC) $(CFLAGS) -o tester tester.c
