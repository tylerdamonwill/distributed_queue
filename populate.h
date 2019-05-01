#if !defined(POPULATE_H)
#define POPULATE_H

#include <string.h>

#define TOTALSONGS 8

typedef struct Song {
  char title[50];
  char filename[50];
  char artist[50];
  int length;
} Song;

void populateLibrary(Song* songs){

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

  strcpy(songs[4].title, "Old Town Road");
  strcpy(songs[4].filename, "oldtownroad");
  strcpy(songs[4].artist, "Lil Nas X");
   
  strcpy(songs[5].title, "22");
  strcpy(songs[5].filename, "22");
  strcpy(songs[5].artist, "Taylor Swift");
   
  strcpy(songs[6].title, "Formation");
  strcpy(songs[6].filename, "formation");
  strcpy(songs[6].artist, "Beyonce");
   
  strcpy(songs[7].title, "Sunflower");
  strcpy(songs[7].filename, "sunflower");
  strcpy(songs[7].artist, "Post Malone");

}

#endif