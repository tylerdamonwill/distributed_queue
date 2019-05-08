#include "populate.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>  
#include <sys/wait.h>

// Function modeled after https://www.geeksforgeeks.org/remove-spaces-from-a-given-string/
void removeSpacesandLowerCase(char *str) { 
  // To keep track of non-space character count 
  int count = 0; 
  
  // Traverse the given string. If current character is not space, then place it at index 'count++'
  // If character is upper case, lowercase the character
  for (int i = 0; str[i]; i++) {
    if (str[i] != ' ' && str[i] != '\n') {
      if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] = str[i] + 32;
      } 
      str[count++] = str[i];
    }
  }
  // Add null terminating character
  str[count] = '\0';
} 

// Play a song and print out songs that "song" parameter is a substring of
void playSong(char *song) {
  // Create and populate song library
  Song songs[TOTALSONGS];
  populateLibrary(songs);

  // Create command and extension string that will be forked
  char command[100], ext[7];
  strcpy(command, "music_library/");
  strcpy(ext, ".mp3");

  // Remove spaces and lowercase string
  removeSpacesandLowerCase(song);

  // Create possible songs array that will be populated with songs that "song" parameter is a substring of
  Song possibleSongs[TOTALSONGS];
  int count = 0;

  // For all songs in the library
  for(int i = 0; i < TOTALSONGS; i++){
    // Compare song string with song in library
    char *check = strstr(songs[i].filename, song);

    // If "song" parameter is equivalent to a song in the library, fork program and play the song
    if(strcmp(song, songs[i].filename) == 0) {
      printf("Now Playing: %s by %s\n", songs[i].title, songs[i].artist);
      strcat(command, song);

      strcat(command, ext);
  
      char *args[] = {"mplayer", command, NULL};

      pid_t c_pid, pid;
      int status;

      c_pid = fork();

      // Child process, play song
      if (c_pid == 0) {
        execvp( args[0], args);
    
        // Only get here if exec failed
        perror("execve failed");
      } else if (c_pid > 0) {
        // Parent process, wait until song finishes playing and print "Song finished playing"
        if ( (pid = wait(&status)) < 0) {
          perror("wait");
          exit(2);
        }

        printf("Song finished playing\n");
      }
      else {
        // Fork failed
        perror("fork failed");
        exit(2);
      }

      return;
      
    } else if (check != NULL) {
      // "song" was found to be a substring of song in library, add it to possibleSongs array
      possibleSongs[count] = songs[i];
      count++;
    }
  }

  if(count != 0){
    // "song" was not found in library, but it was a substring of "count" songs in the library, print those songs out 
    for(int i = 0; i < count; i++){
      printf("Did you mean '%s' by '%s'\n", possibleSongs[i].title, possibleSongs[i].artist);
    }
  } else if (count == 0){
    // "song" was not a substring of any songs in the library, ask the user to enter a valid song
    printf("Please enter a valid song\n");
  }
  return;
}

// Check is a song is in the song library and print out songs that "song" parameter is a substring of
bool inLibrary(char *song) {
  // Create and populate song library
  Song songs[TOTALSONGS];
  populateLibrary(songs);

  // Remove spaces and lowercase string
  removeSpacesandLowerCase(song);

  // Create possible songs array that will be populated with songs that "song" parameter is a substring of
  Song possibleSongs[TOTALSONGS];
  int count = 0;

  // For all songs in the library
  for(int i = 0; i < TOTALSONGS; i++){
    // Compare song string with song in library
    char *check = strstr(songs[i].filename, song);
    
    // If "song" parameter is equivalent to a song in the library, return true and print no other possible matches
    if(strcmp(song, songs[i].filename) == 0) {
      return true;
    } else if (check != NULL) {
      // "song" was found to be a substring of song in library, add it to possibleSongs array
      possibleSongs[count] = songs[i];
      count++;
    }
  }

  // "song" was not found in library, but it was a substring of "count" songs in the library, print those songs out
  if(count != 0){
    for(int i = 0; i < count; i++){
      printf("Did you mean '%s' by '%s'\n", possibleSongs[i].title, possibleSongs[i].artist);
    }
    return false;
  } else if (count == 0){ // Else, prompt user for a valid song input
    printf("Please enter a valid song\n");
    return false;
  }
  return false;
}

// Print all songs in the library
void printLibrary() {  
  Song songs[TOTALSONGS];
  populateLibrary(songs);

  printf( "Library Includes\n");
  for(int i = 0; i < TOTALSONGS; i++){
      // Send a song
    char curSong[255];
    strcpy(curSong, "'");
    strcat(curSong, songs[i].title);
    strcat(curSong, "' by '");
    strcat(curSong, songs[i].artist);
    strcat(curSong, "'");
    printf("%s\n", curSong);
  }
  
}
