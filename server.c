#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <stdbool.h>
#include "playSong.h"
#include "populate.h"

#include "socket.h"

#define BUFFER_LEN 256

typedef struct node {
  char song_name[BUFFER_LEN];
  struct node* next;
}node_t;

typedef struct queue {
  struct node* head;
} queue_t;

//error checker
void errorChecker(int rtrVal){
  if(rtrVal != 0){
    perror("locking failed ");
    exit(2);
  }
}

void queue_put(queue_t* queue, char * element) {

  node_t* newNode = (node_t*)malloc(sizeof(node_t));
  strcpy(newNode->song_name, element);

  node_t *cur = queue->head;
  if(cur == NULL) {
    queue->head = newNode;
  } else {
    while (cur->next != NULL){
      cur = cur->next;
    }
    cur->next = newNode;
  }

}

char * queue_take(queue_t* queue) {
  node_t* temp = queue->head;

  if(temp == NULL){
    return "";
  }

  char static rtr[BUFFER_LEN];
  strcpy(rtr, queue->head->song_name);
  queue->head = queue->head->next;
  free(temp);
  return rtr;
}

// Initialize a new queue


// Create the music queue
queue_t* music_queue;


void * musicHandler (){
  while(1){
    while(music_queue->head != NULL){
      playSong(music_queue->head->song_name);
      queue_take(music_queue);
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

     removeSpacesandLowerCase(read_message);

     Song songs[TOTALSONGS];
     populateLibrary(songs);

     Song possibileSongs[TOTALSONGS];
     int count = 0;

     for(int i = 0; i < TOTALSONGS; i++){

      char *check = strstr(songs[i].filename, read_message);

      if(strcmp(read_message, songs[i].filename) == 0) {
        queue_put(music_queue, read_message);
      } else if (check != NULL) {
        possibileSongs[count] = songs[i];
        count++;
      }
    }

    if(count != 0){
      for(int i = 0; i < count; i++){
        char str[BUFFER_LEN];
        strcpy(str, "Did you mean '");
        strcat(str, possibileSongs[i].title);
        strcat(str, "' by '");
        strcat(str, possibileSongs[i].artist);
        strcat(str, "'\n");

        // Send a message to the client
        fprintf(to_client, "%s", str);

        // Flush the output buffer
        fflush(to_client);
      }
    } else if (count == 0){
       // Send a message to the client
      fprintf(to_client, "%s", "Please enter a valid song\n");

      // Flush the output buffer
      fflush(to_client);
    }

  }

  for(int i = 0; i < strlen(read_message); i++){
    write_message[i] = read_message[i];
  }

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





















