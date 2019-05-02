#include "playSong.h"

int main() {

  char song[50];

  printf("Enter song: ");
  fgets(song, sizeof(song), stdin);
  for(int i = 49; i >= 0; i--){
    if(song[i] == '\n'){
      song[i] = '\0';
    }
  }

  printf("%s\n", song);
  printf("Here %d\n", inLibrary(song));
  printf("%s\n", song);
  
  return 0;
}
