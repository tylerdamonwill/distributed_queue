#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "playSong.h"

#include "socket.h"

#define BUFFER_LEN 256

typedef struct node {
  char song_name[BUFFER_LEN];
  struct node* next;
}node_t;

typedef struct queue {
  struct node* head;
}queue_t;

// Create the music queue
queue_t* music_queue;


void * musicHandler (){
  while(1){
    while(music_queue->head != NULL){
      printf("Song: %s\n", music_queue->head->song_name);
      playSong(music_queue->head->song_name);
    }
  }
}


void * clientHandler(void* arg){

  int client_socket_fd = * (int *) arg;  
	
  //printf("In clientHandler: One client connected! | %d\n", client_socket_fd);
  
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
  char read_message[BUFFER_LEN] = "";
  char write_message[BUFFER_LEN];

  while(strcmp(read_message,"quit\n")) {
    // Get inputs from stdin
    if(fgets(read_message, BUFFER_LEN, from_client) == NULL) {
      perror("Reading from client failed");
      exit(2);
    }
    
   printf("Client sent: %s", read_message);


   if(read_message[0] == 'a' && read_message[1] == 'd' && read_message[2] == 'd'){
     // Substitute "add" as "   "
     read_message[0] = ' ';
     read_message[1] = ' ';
     read_message[2] = ' ';

     node_t* song_node = malloc(sizeof(node_t));
     strcpy(song_node->song_name, read_message);
     printf("here\n");
     song_node->next = NULL;
     music_queue->head = song_node;
    }
   
    for(int i = 0; i < strlen(read_message); i++){
      write_message[i] = toupper(read_message[i]);
    }

    // Send a message to the client
    fprintf(to_client, "%s", write_message);
  
    // Flush the output buffer
    fflush(to_client);

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

  // Start listening for connections, with a maximum of five queued connection
  if(listen(server_socket_fd, 5)) {
    perror("listen failed");
    exit(2);
  }
	
  printf("Server listening on port %u\n", port);

  music_queue = malloc(sizeof(struct queue));
  music_queue->head = NULL;
  
  pthread_t playing_thread;
  pthread_create(&playing_thread, NULL, &musicHandler, NULL);
  
  // An infinite loop that accepts an infinite amount of clients
  while(1){
    // Wait for a client to connect
    int client_socket_fd = server_socket_accept(server_socket_fd);
    if(client_socket_fd == -1) {
      perror("accept failed");
      exit(2);
    }

    pthread_t thr;

    printf("In Main: One client connected! | %d\n", client_socket_fd);
    pthread_create(&thr, NULL, &clientHandler, &client_socket_fd);
  }

  return 0;
}





















