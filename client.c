#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "socket.h"

#define BUFFER_LEN 256

int main(int argc, char** argv) {
  if(argc != 4) {
    fprintf(stderr, "Usage: %s <user name> <server name> <port>\n", argv[0]);
    exit(1);
  }
	
  // Read command line arguments
  char* user_name = argv[1];
  char* server_name = argv[2];
  unsigned short port = atoi(argv[3]);
	
  // Connect to the server
  int socket_fd = socket_connect(server_name, port);
  if(socket_fd == -1) {
    perror("Failed to connect");
    exit(2);
  }
  
  // Set up file streams
  FILE* to_server = fdopen(dup(socket_fd), "wb");
  if(to_server == NULL) {
    perror("Failed to open stream to server");
    exit(2);
  }
  
  FILE* from_server = fdopen(dup(socket_fd), "rb");
  if(from_server == NULL) {
    perror("Failed to open stream from server");
    exit(2);
  }


  char message[BUFFER_LEN];
  fgets(message, BUFFER_LEN, stdin);

  while(strcmp(message, "quit\n")){

    // Send a message to the server
    fprintf(to_server, "%s", message);
  
    // Flush the output buffer
    fflush(to_server);

    // Read a message from the server
    char buffer[BUFFER_LEN];
    if(fgets(buffer, BUFFER_LEN, from_server) == NULL) {
      perror("Reading from server failed");
      exit(2);
    }
  
    printf("Server: %s", buffer);

    fgets(message, BUFFER_LEN, stdin);
  }

  // Send a message to the server
  fprintf(to_server, "%s", message);
  
  // Flush the output buffer
  fflush(to_server);
  
  // Close file streams
  fclose(to_server);
  fclose(from_server);
  
  // Close socket
  close(socket_fd);
	
  return 0;
}


















