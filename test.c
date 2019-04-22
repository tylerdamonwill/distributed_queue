//#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
   char command[50];

<<<<<<< HEAD
   while(1){
=======
int main ()
{

  printf("test\n");
   // init FMOD sound system
   FSOUND_Init (44100, 32, 0);
>>>>>>> dfdf14e46b5fd6d72b56ecb2ec9acd10580710ec

   strcpy( command, "afplay music_library/ice.mp3 &" );
   system(command);

   sleep(5);

<<<<<<< HEAD
   strcpy( command, "killall afplay" );
   system(command);
}   

   return(0);
} 
=======
   // clean up
   FSOUND_Sample_Free (handle);
   FSOUND_Close();
}
>>>>>>> dfdf14e46b5fd6d72b56ecb2ec9acd10580710ec
