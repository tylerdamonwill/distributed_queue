#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "playSong.h"
#include "socket.h"


//global testing variable to fetch time
struct timeval stop, start;

// This is a hard limit on how many bits the server and clients can send to each other
#define BUFFER_LEN 255

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
  printf("\nHi, I'm a clever AI that handles the fun Musi-Q you share with other cs students!\nTo add a song to the Musi-Q, type \"add <name of the song>\" \nTo view the library, type \"view library\" \nTo view the current Musi-Q, type \"view queue\" \nTo quit, type \"quit\" \n");
  
  char write_message[BUFFER_LEN] = "";
  char read_message[BUFFER_LEN];
 
  while(strcmp(write_message, "quit\n")){
    // Get inputs from stdin
    if(fgets(write_message, BUFFER_LEN, stdin) == NULL) {
      perror("Reading from client failed");
      exit(2);
    }

    // Lower case string from client
    for(int i = 0; i < strlen(write_message); i++){
      if (write_message[i] >= 'A' && write_message[i] <= 'Z') {
      write_message[i] = write_message[i] + 32;
      } 
    }
    
    // Make sure the input is not just '\n'
    while (write_message[0] == '\n'){
      printf("Don't send an empty message to the server, type \"add <name of the song>\" or \"view library\" or \"view queue\" or \"quit\" \n");
      fgets(write_message, BUFFER_LEN, stdin);
    }
  
    // If the input is add, parse the input before send it to the server
    if(write_message[0] == 'a' && write_message[1] == 'd' && write_message[2] == 'd'){
      
      // Substitute the substring "add" with spaces
      write_message[0] = ' ';
      write_message[1] = ' ';
      write_message[2] = ' ';

      // If the input song is in library
      if(inLibrary(write_message)){
        // Send the add message to the server
        fprintf(to_server, "%s\n", write_message);
        fflush(to_server);
        
      } else { // If the input song is not in library
        
        // Send the add message to the server
        fprintf(to_server, "%s\n", "Invalid input");
        fflush(to_server);
      }
      
      // Read a message from the server after sending the add message
      if(fgets(read_message, BUFFER_LEN, from_server) == NULL) {
        perror("Reading from server failed");
        exit(2);
      }
      
      // Print the message from server
      printf("Clever AI: %s", read_message);
    }

    // If the input is view library
    else if (strcmp(write_message, "view library\n") == 0){
      
      // Print the library
      printLibrary();
    }
    
    // If the input is view queue
    else if (strcmp(write_message, "view queue\n") == 0){
      
      // Send the view message to the server
      fprintf(to_server, "%s\n", write_message);
      fflush(to_server);

      // Get the queue information from the server
      if(fgets(read_message, BUFFER_LEN, from_server) == NULL) {
        perror("Reading from server failed");
        exit(2);
      }
      
      // Print the current music queue to client's terminal
      while (strcmp(read_message, "End of queue\n") != 0){
        printf("%s", read_message);
        if(fgets(read_message, BUFFER_LEN, from_server) == NULL) {
        perror("Reading from server failed");
        exit(2);
        }
      }
      printf("%s", read_message);
      
    } else {
      printf("INVALID INPUT: type \"add <name of the song>\" or \"view library\" or \"view queue\" or \"quit\"\n");
    }
  }

  // Send the add message to the server
  fprintf(to_server, "%s\n", "quit");
  
  // Flush the add message to_server buffer
  fflush(to_server);

  // Send a useful quit message
  printf("quitting...\n");  
  
  // Close file streams
  fclose(to_server);
  fclose(from_server);
  
  // Close socket
  close(socket_fd);
  
  return 0;
}
