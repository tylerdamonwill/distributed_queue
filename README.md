# Musi-Q

The Musi-Q is a distributed system that allows users to connect to a central server and play music. The central server plays the first song in the queue and displays the song that is being currently played along with the next song. Users can add songs to the queue and view the contents of the queue. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Make sure you have mplayer installed on your computer. Run

```
mplayer version
```
on the machine that will be running the server to make sure you have a copy of mplayer to successfully play music.

### Installing

Step 1: Clone the repo

```
git clone https://github.com/tylerdamonwill/distributed_queue.git
```

Step 2: Run the Make file

```
make
```

Step 3 (Server side): Start the server
```
./server
```
TO DO: Gif of server side

Step 3 (Server side): Start a client
```
./client
```
TO DO: Client side

## Authors

* **Anh Thu Pham** - (https://github.com/PhamAnhThu)
* **Zoe Cui** - (https://github.com/cuiqingq)
* **Tyler Williams** - (https://github.com/tylerdamonwill)

## Acknowledgments

* Charlie Curtsinger, Maddie Kirwin, Garrett Wang
  * general project help
* https://gist.github.com/PurpleBooth/109311bb0361f32d87a2
  * writing a good README.md
* https://www.usna.edu/Users/cs/aviv/classes/ic221/s16/lec/14/lec.html
  * help with fork and exec
* https://stackoverflow.com/questions/27306764/capturing-exit-status-code-of-child-process/27307246#27307246
  * help with fork and exec
