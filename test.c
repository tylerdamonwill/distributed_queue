#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h> 
#include <errno.h>   
#include <sys/wait.h>

#define TOTALSONGS 5

//https://www.usna.edu/Users/cs/aviv/classes/ic221/s16/lec/14/lec.html

//https://stackoverflow.com/questions/27306764/capturing-exit-status-code-of-child-process/27307246#27307246

void removeSpaces(char *str) // https://www.geeksforgeeks.org/remove-spaces-from-a-given-string/
{ 
  // To keep track of non-space character count 
  int count = 0; 
  
  // Traverse the given string. If current character 
  // is not space, then place it at index 'count++' 
  for (int i = 0; str[i]; i++) 
    if (str[i] != ' ') 
      str[count++] = str[i]; // here count is 
  // incremented 
  str[count] = '\0'; 
} 

typedef struct Song {
  char title[50];
  char filename[50];
  char artist[50];
  int length;
} Song;

Song songs[TOTALSONGS];

void populateLibrary(){
  strcpy(songs[0].title, "Still Here");
  strcpy(songs[0].filename, "stillhere");
  strcpy(songs[0].artist, "Drake");

  strcpy(songs[1].title, "Ice Ice Baby");
  strcpy(songs[1].filename, "iceicebaby");
  strcpy(songs[1].artist, "Vanilla Ice");

  strcpy(songs[2].title, "Money Trees");
  strcpy(songs[2].filename, "moneytrees");
  strcpy(songs[2].artist, "Kendrick Lamar");

  strcpy(songs[3].title, "Neighbors");
  strcpy(songs[3].filename, "neighbors");
  strcpy(songs[3].artist, "J. Cole");

  strcpy(songs[0].title, "Late");
  strcpy(songs[0].filename, "late");
  strcpy(songs[0].artist, "Tyler");
}

int main(int argc, char *argv[]) {
  populateLibrary();
  
  char song[100], command[15], ext[5];

  strcpy(command, "music_library/");
  strcpy(ext, ".mp3");

  printf("Enter song: ");
  fgets(song, sizeof(song), stdin);
  for(int i = 49; i >= 0; i--){
    if(song[i] == '\n'){
      song[i] = '\0';
    }
  }

  removeSpaces(song);

  Song possibileSongs[TOTALSONGS];
  int count = 0;

  for(int i = 0; i < TOTALSONGS; i++){

    char *check = strstr(songs[i].filename, song);

    if(strcmp(song, songs[i].filename) == 0) {
      printf("Now Playing: %s by %s\n", songs[i].title, songs[i].artist);
      //printf("PID of hello.c = %d\n", getpid());
      strcat(command, song);
      strcat(command, ext);
  
      char *args[] = {"mplayer", command, NULL};

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

      return 0;
      
    } else if (check != NULL) {
      possibileSongs[count] = songs[i];
      count++;
    }
  }

  if(count != 0){
    for(int i = 0; i < count; i++){
      printf("Did you mean '%s' by '%s'\n", possibileSongs[i].title, possibileSongs[i].artist);
    }
    exit(2);//needs to change later
  } else if (count == 0){
    printf("Please enter a valid song\n");
    exit(2);//needs to change later
  }

  return 0;
}
