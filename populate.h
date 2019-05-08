#if !defined(POPULATE_H)
#define POPULATE_H

#include <string.h>

#define TOTALSONGS 26

typedef struct Song {
  char title[50];
  char filename[50];
  char artist[50];
  int length;
} Song;

// Populate song library
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

  strcpy(songs[8].title, "Sponge");
  strcpy(songs[8].filename, "sponge");
  strcpy(songs[8].artist, "Post Malone");
  
  strcpy(songs[9].title, "Dog");
  strcpy(songs[9].filename, "dog");
  strcpy(songs[9].artist, "Posty");

  strcpy(songs[10].title, "Boondocks Theme Song");
  strcpy(songs[10].filename, "boondocksthemesong");
  strcpy(songs[10].artist, "Asheru");

  strcpy(songs[11].title, "Kill This Love");
  strcpy(songs[11].filename, "killthislove");
  strcpy(songs[11].artist, "BLACKPINK");

  strcpy(songs[12].title, "The Lazy Song");
  strcpy(songs[12].filename, "thelazysong");
  strcpy(songs[12].artist, "Bruno Mars");

  strcpy(songs[13].title, "Boy With Luv");
  strcpy(songs[13].filename, "boywithluv");
  strcpy(songs[13].artist, "BTS");

  strcpy(songs[14].title, "Havana");
  strcpy(songs[14].filename, "havana");
  strcpy(songs[14].artist, "Camila Cabello");

  strcpy(songs[15].title, "Friends Theme Song");
  strcpy(songs[15].filename, "friendsthemesong");
  strcpy(songs[15].artist, "A Friends Writer");

  strcpy(songs[16].title, "Full House Theme Song");
  strcpy(songs[16].filename, "fullhousethemesong");
  strcpy(songs[16].artist, "A Full House Writer");

  strcpy(songs[17].title, "How Deep is Your Love");
  strcpy(songs[17].filename, "howdeepisyourlove");
  strcpy(songs[17].artist, "PJ Morton");

  strcpy(songs[18].title, "Moon Leaf");
  strcpy(songs[18].filename, "moonleaf");
  strcpy(songs[18].artist, "Rex Orange County");

  strcpy(songs[19].title, "I'm a Goofy Goober");
  strcpy(songs[19].filename, "imagoofygoober");
  strcpy(songs[19].artist, "Spongebob");

  strcpy(songs[20].title, "Spongebob Theme Song");
  strcpy(songs[20].filename, "spongebobthemesong");
  strcpy(songs[20].artist, "Spongebob");

  strcpy(songs[21].title, "Suspense");
  strcpy(songs[21].filename, "suspense");
  strcpy(songs[21].artist, "Scary Dude");

  strcpy(songs[22].title, "The Campfire Song");
  strcpy(songs[22].filename, "thecampfiresong");
  strcpy(songs[22].artist, "Spongebob");

  strcpy(songs[23].title, "The Krusty Krab Pizza Song");
  strcpy(songs[23].filename, "thekrustykrabpizzasong");
  strcpy(songs[23].artist, "Spongebob");

  strcpy(songs[24].title, "See You Again");
  strcpy(songs[24].filename, "seeyouagain");
  strcpy(songs[24].artist, "Wiz Khalifa");
  
  strcpy(songs[25].title, "Yee");
  strcpy(songs[25].filename, "yee");
  strcpy(songs[25].artist, "Maddie Kirwin");
}

#endif
