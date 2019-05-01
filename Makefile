CC := clang 
CFLAGS := -g

all: server client playSong

clean:
	rm -rf server client test server.dSYM client.dSYM

server: server.c socket.h
	$(CC) $(CFLAGS) -o server server.c -lpthread

client: client.c
	$(CC) $(CFLAGS) -o client client.c -lpthread

playSong: playSong.c
	$(CC) $(CFLAGS) -o play playSong.c
