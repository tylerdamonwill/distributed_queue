//#include <iostream>
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

   /*
int main () {

   char command[50];
warning: implicit declaration of function 'gets' is invalid in C99

   while(){

   strcpy( command, "afplay music_library/StillHere.mp3 &" );
   system(command);

   sleep(5);

   strcpy( command, "killall afplay" );
   system(command);
}   */

//https://www.usna.edu/Users/cs/aviv/classes/ic221/s16/lec/14/lec.html

//https://stackoverflow.com/questions/27306764/capturing-exit-status-code-of-child-process/27307246#27307246

int main(int argc, char *argv[]) {
   char song[50];

   printf("Enter song: ");
   fgets(song, sizeof(song), stdin);
   for(int i = 49; i >= 0; i--){
     if(song[i] == '\n'){
       song[i] = '\0';
     }
   }

   char *args[] = {song, NULL};

   pid_t c_pid, pid;
   int status;

   c_pid = fork();

   if (c_pid == 0) {
      execvp( "./input", args);
    //only get here if exec failed                                                                                                                                             
      perror("execve failed");
   } else if (c_pid > 0) {
        if ( waitpid(c_pid, &status, 0) == -1 ) {
        perror("waitpid failed");
        return EXIT_FAILURE;
    }

    if ( WIFEXITED(status) ) {
        const int es = WEXITSTATUS(status);
        printf("exit status was %d\n", es);
    }
   }
    else {
   perror("fork failed");
   _exit(1);
}

return 0;
}
