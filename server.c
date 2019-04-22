#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>

#include "socket.h"

#define BUFFER_LEN 256
#define NUM 5

void * clientFunc(void * arg){

  int client_socket_fd = * (int *) arg;  
	
  printf("Client connected! | %d\n", client_socket_fd);
  
  // Set up file streams to access the socket
  FILE* to_client = fdopen(dup(client_socket_fd), "wb");
  if(to_client == NULL) {
    perror("Failed to open stream to client");
    exit(2);
  }
  
  FILE* from_client = fdopen(dup(client_socket_fd), "rb");
  if(from_client == NULL) {
    perror("Failed to open stream from client");
    exit(2);
  }
	

  // Receive a message from the client
  char buffer[BUFFER_LEN];
  if(fgets(buffer, BUFFER_LEN, from_client) == NULL) {
    perror("Reading from client failed");
    exit(2);
  }

  while(strcmp(buffer,"quit\n")) {
    printf("Client sent: %s\n", buffer);

    for(int i = 0; i < strlen(buffer); i++){
      buffer[i] = toupper(buffer[i]);
    }

    // Send a message to the client
    fprintf(to_client, "%s", buffer);
  
    // Flush the output buffer
    fflush(to_client);

    fgets(buffer, BUFFER_LEN, from_client);
  }


  // Close file streams
  fclose(to_client);
  fclose(from_client);
  // Close sockets
  close(client_socket_fd);

  return NULL;
}

int main() {
  // Open a server socket
  unsigned short port = 0;
  int server_socket_fd = server_socket_open(&port);
  if(server_socket_fd == -1) {
    perror("Server socket was not opened");
    exit(2);
  }
  pthread_t thr[NUM];

  // Start listening for connections, with a maximum of one queued connection
  if(listen(server_socket_fd, 5)) {
    perror("listen failed");
    exit(2);
  }
	
  printf("Server listening on port %u\n", port);

  
  int count = 0;
  while(count < NUM){
    // Wait for a client to connect
    int client_socket_fd = server_socket_accept(server_socket_fd);
    if(client_socket_fd == -1) {
      perror("accept failed");
      exit(2);
    }

    printf("Main Client connected! | %d\n", client_socket_fd);
    pthread_create(&thr[count], NULL, &clientFunc, &client_socket_fd);
    count++;
  }

  
  for(int i = 0; i < NUM; i++){
    pthread_join(thr[i], NULL);
  }

  close(server_socket_fd);
	
  return 0;
}





















