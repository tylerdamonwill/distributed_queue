#include "populate.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/wait.h>

//https://www.usna.edu/Users/cs/aviv/classes/ic221/s16/lec/14/lec.html

//https://stackoverflow.com/questions/27306764/capturing-exit-status-code-of-child-process/27307246#27307246

void removeSpacesandLowerCase(char *str) // https://www.geeksforgeeks.org/remove-spaces-from-a-given-string/
{ 
  // To keep track of non-space character count 
  int count = 0; 
  
  // Traverse the given string. If current character 
  // is not space, then place it at index 'count++' 
  for (int i = 0; str[i]; i++) {
    if (str[i] != ' ' && str[i] != '\n') {
      if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] = str[i] + 32;
      } 
      str[count++] = str[i]; // here count is 
    }
  }
  // incremented 
  str[count] = '\0';
} 


void playSong(char *song) {
  Song songs[TOTALSONGS];
  populateLibrary(songs);
  
  char command[100], ext[7];

  strcpy(command, "music_library/");
  strcpy(ext, ".mp3");

  removeSpacesandLowerCase(song);

  Song possibileSongs[TOTALSONGS];
  int count = 0;

  for(int i = 0; i < TOTALSONGS; i++){

    char *check = strstr(songs[i].filename, song);

    if(strcmp(song, songs[i].filename) == 0) {
      printf("Now Playing: %s by %s\n", songs[i].title, songs[i].artist);
      strcat(command, song);

      strcat(command, ext);
  
      char *args[] = {/*"afplay"*/"mplayer", command, NULL};

      pid_t c_pid, pid;
      int status;

      c_pid = fork();

      if (c_pid == 0) {
        execvp( args[0], args);
    
        //only get here if exec failed

        perror("execve failed");
      } else if (c_pid > 0) {

        if ( (pid = wait(&status)) < 0) {
          perror("wait");
          exit(2);
        }

        printf("Song finished playing\n");
      }
      else {
        perror("fork failed");
        exit(2);
      }

      return;
      
    } else if (check != NULL) {
      possibileSongs[count] = songs[i];
      count++;
    }
  }

  if(count != 0){
    for(int i = 0; i < count; i++){
      printf("Did you mean '%s' by '%s'\n", possibileSongs[i].title, possibileSongs[i].artist);
    }
  } else if (count == 0){
    printf("Please enter a valid song\n");
  }

  return;
}
