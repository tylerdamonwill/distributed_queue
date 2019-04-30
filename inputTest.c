#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TOTALSONGS 5

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
	strcpy(songs[1].filename, "ice");
	strcpy(songs[1].artist, "Vanilla Ice");

	strcpy(songs[2].title, "Money Trees");
	strcpy(songs[2].filename, "moneytrees");
	strcpy(songs[2].artist, "Kendrick Lamar");

	strcpy(songs[3].title, "Neighbors");
	strcpy(songs[3].filename, "neighbors");
	strcpy(songs[3].artist, "J. Cole");

        strcpy(songs[0].title, "Late");
	strcpy(songs[0].filename, "late");
	strcpy(songs[0].artist, "Drake");
}

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

int main(int argc, char *argv[]) {
	populateLibrary();

	char command[50], extension[7];
	strcpy(command, "mplayer music_library/");
	strcpy(extension, ".mp3 &");

	Song possibileSongs[TOTALSONGS];
	int count = 0;


	removeSpaces(argv[0]);
        printf("args %s\n", argv[0]);

        //figuer this out tomorrow


	for(int i = 0; i < TOTALSONGS; i++){

		char *check = strstr(songs[i].filename, argv[0]);

		if(strcmp(argv[0], songs[i].filename) == 0) {
			printf("Now Playing: %s by %s\n", songs[i].title, songs[i].artist);
				//printf("PID of hello.c = %d\n", getpid());
			strcat(command, songs[i].filename);
			strcat(command, extension);
			system(command);
			return(0);
		} else if (check != NULL) {
			possibileSongs[count] = songs[i];
			count++;
		}
	}

	if(count != 0){
		for(int i = 0; i < count; i++){
			printf("Did you mean '%s' by '%s'\n", possibileSongs[i].title, possibileSongs[i].artist);
		}
		return 0;
	} else {
		printf("Please enter a valid song\n");
		return 0;
	}

	return 0;
}
