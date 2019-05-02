#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "playSong.h"
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

  // Print instructions of how to use the program
  printf("\nHi, I'm a clever AI that handles the fun music queue you share with other cs students!\nTo add a song, type \"add <name of the song>\" ;\nTo quit, type \"quit\" \n");
  
  char write_message[BUFFER_LEN] = "";
  char read_message[BUFFER_LEN];
 
  while(strcmp(write_message, "quit\n")){

    // Get inputs from stdin
    if(fgets(write_message, BUFFER_LEN, stdin) == NULL) {
      perror("Reading from client failed");
      exit(2);
    }
    
    // Make sure the input is not just '\n'
    while (write_message[0] == '\n'){
      printf("Don't send an empty message to the server, type \"add <name of the song>\" or \"quit\" \n");
      fgets(write_message, BUFFER_LEN, stdin);
    }
  
    // if the input is add
    if(write_message[0] == 'a' && write_message[1] == 'd' && write_message[2] == 'd'){
     write_message[0] = ' ';
     write_message[1] = ' ';
     write_message[2] = ' ';

      if(inLibrary(write_message)){
      // Send the add message to the server
      fprintf(to_server, "%s\n", write_message);
  
      // Flush the add message to_server buffer
      fflush(to_server);
      } else {
        fprintf(to_server, "%s\n", "Invalid input");
  
      // Flush the add message to_server buffer
      fflush(to_server);
    }
              // Send the add message to the server
      //fprintf(to_server, "%s", "Client entered invalid input");
  
      // Flush the add message to_server buffer
     // fflush(to_server);
     // printf("Here MOE\n");
      //}

      // Read a message from the server
      if(fgets(read_message, BUFFER_LEN, from_server) == NULL) {
        perror("Reading from server failed");
        exit(2);
      }
      printf("Clever AI: %s", read_message);
    }
  }

  // Send a useful quit message
  printf("quitting...\n");  
  
  // Close file streams
  fclose(to_server);
  fclose(from_server);
  
  // Close socket
  close(socket_fd);
  
  return 0;
}
