//#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
   char command[50];

   while(1){

   strcpy( command, "afplay music_library/ice.mp3 &" );
   system(command);

   sleep(5);

   strcpy( command, "killall afplay" );
   system(command);
}   

   return(0);
} 