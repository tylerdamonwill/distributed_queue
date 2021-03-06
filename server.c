#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <stdbool.h>

#include "playSong.h"
#include "socket.h"


// Clients can only quit by typing "quit"; if a client try to quit via ctrl-c, the server will crush and there will be a "connection reset by peer" error message shown on the client side.


// This is a hard limit on how many bits the server and clients can send to each other
#define BUFFER_LEN 255

// Structure that holds song name and the information of the next node
typedef struct node {
  char song_name[BUFFER_LEN];
  struct node* next;
}node_t;

// Structure that holds the first song node
typedef struct queue {
  struct node* head;
} queue_t;

// Create the music queue
queue_t* music_queue;

// Error checker function
void errorChecker(int rtrVal){
  if(rtrVal != 0){
    perror("locking failed ");
    exit(2);
  }
}

//create global lock
pthread_mutex_t lock;

// This function puts an element at the end of a queue
void queue_put(queue_t* queue, char * element) {
  // Lock global lock 
  if(pthread_mutex_lock(&lock)) exit(2);
  
  // Create a new node with the song's name
  node_t* newNode = (node_t*)malloc(sizeof(node_t));
  strcpy(newNode->song_name, element);
  newNode->next = NULL;

  node_t *cur = queue->head;
  
  if(cur == NULL) {

    // If the queue is empty, just let head be the new node
    queue->head = newNode;
  } else {   // If the queue is not empty

    while (cur->next != NULL){
      // If the current node is not the last node in the queue, move the cur pointer to the next node
      cur = cur->next;
    }
    // If the current node is the last node in the queue, add the new node after the current node
    cur->next = newNode;
  }
  // Unlock global lock 
  if(pthread_mutex_unlock(&lock)) exit(2);
}

// This function takes the first element off a queue
void queue_take(queue_t* queue) {
  // Lock global lock 
  if(pthread_mutex_lock(&lock)) exit(2);
  
  node_t* temp = queue->head;
  
  // If the queue is empty, there is no element to dequeue, do nothing
  if(temp == NULL){
    // Unlock global lock 
  if(pthread_mutex_unlock(&lock)) exit(2);
    return;
  } else {
    
  // If the queue is not empty, dequeue the first node
  queue->head = queue->head->next;
  free(temp);
  }
  // Unlock global lock 
  if(pthread_mutex_unlock(&lock)) exit(2);
}

// This function prints songs from the queue
void queue_print(queue_t* queue, FILE* to_client) {
  // Lock global lock 
  if(pthread_mutex_lock(&lock)) exit(2);
  
  node_t* cur = queue->head;
  fprintf(to_client, "%s", "Queue Includes-------------------------------\n");
  fflush(to_client);

  // Populate songs to compare with songs in queue
  Song songs[TOTALSONGS];
  populateLibrary(songs);
  char songName[BUFFER_LEN];
  
  while (cur != NULL){
    // If the current node is not the last node in the queue, send cur->song_name to client and move the cur pointer to the next node

    // Find cur->song_name in queue and print the formatted song title name with corresponding artist
    for (int i = 0; i < TOTALSONGS; i++){
      if(strncmp(songs[i].filename, cur->song_name, strlen(songs[i].filename)) == 0){
        strcpy(songName, "'");
        strcat(songName, songs[i].title);
        strcat(songName, "' by '");
        strcat(songName, songs[i].artist);
        strcat(songName, "'");
      }
    }
    fprintf(to_client, "%s\n", songName);
    fflush(to_client);
    cur = cur->next;
  }

  // Send deliminating string to client when the end of queue is reached
  fprintf(to_client, "%s", "End of queue\n");
  fflush(to_client);

  // Unlock global lock 
  if(pthread_mutex_unlock(&lock)) exit(2);
}

// This thread plays the songs in the global music queue
void * musicHandler (){
  
  // This thread keeps running indefinitely until server quits
  while(1){
    
    // Keeps checking if the music queue is not empty
    while(music_queue->head != NULL){
      
      // Play the first song in the queue, and dequeue the first song
      playSong(music_queue->head->song_name);
      queue_take(music_queue);
    }
  }
}

// This thread handle a client's request
void * clientHandler(void* arg){

  // Get the client socket file discriptor's number
  int client_socket_fd = * (int *) arg;  
  
  //printf("In clientHandler: One client connected! | %d\n", client_socket_fd);
  
  // Set up file streams to access the client socket
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

  // Set up message buffers to receive from/send to the file streams created above
  char read_message[BUFFER_LEN] = "";
  char write_message[BUFFER_LEN];

  
  // Get inputs from client
  if(fgets(read_message, BUFFER_LEN, from_client) == NULL) {
    perror("Reading from client failed");
    exit(2);
  }

  // Keep handling a client's requests until client enters "quit"
  while(strcmp(read_message, "quit\n")) {

    // If the input is view queue
    if (strcmp(read_message, "view queue\n") == 0) {
      // Print the queue
      queue_print(music_queue, to_client);
      
    }
    // If the input is invalid (songname wise)
    else if (strcmp(read_message, "Invalid input\n") == 0) {
  
      // Send a helpful message to the client
      fprintf(to_client, "%s", "Try another song name, to see what we have in the library, type \"view library\", to see what other people have added to the Musi-Q, type \"view queue\" \n");
      fflush(to_client);
      
    }
    // If client's input is valid
    else if(strcmp(read_message, "\n") != 0) {
      // Put client's input into the global queue
      queue_put(music_queue, read_message);

      // Send a message to the client
      fprintf(to_client, "%s", "Song added successfully: great song choice!\n");
      fflush(to_client);
      
    }

    // Get inputs from client
    if(fgets(read_message, BUFFER_LEN, from_client) == NULL) {
      perror("Reading from client failed");
      exit(2);
    }
  }

  // Close file streams
  fclose(to_client);
  fclose(from_client);
  // Close the socket
  close(client_socket_fd);

  return NULL;
}



int main() {
  // Initialize global lock
  if(pthread_mutex_init(&lock, NULL)) exit(2);
  // Open a server socket
  unsigned short port = 0;
  int server_socket_fd = server_socket_open(&port);
  if(server_socket_fd == -1) {
    perror("Server socket was not opened");
    exit(2);
  }

  // Start listening for connections, with a maximum of five queued connection
  if(listen(server_socket_fd, 5)) {
    perror("Listening failed");
    exit(2);
  }

  // Print the server port number so clients can connect
  printf("Server listening on port %u\n", port);

  // Malloc memory for the global music queue
  music_queue = malloc(sizeof(struct queue));
  music_queue->head = NULL;

  // Create a thread that plays the songs in the global music queue
  pthread_t playing_thread;
  pthread_create(&playing_thread, NULL, &musicHandler, NULL);
  
  // Infinite loop that accepts an indeterminate amount of clients
  while(1){
  
    // Wait for a client to connect
    int client_socket_fd = server_socket_accept(server_socket_fd);
    if(client_socket_fd == -1) {
      perror("Accept failed");
      exit(2);
    }

    // Create a client thread to handle this client's request
    pthread_t client_thread;
    printf("In Main: One client connected! | %d\n", client_socket_fd);
    pthread_create(&client_thread, NULL, &clientHandler, &client_socket_fd);  
  }

  return 0;
}
