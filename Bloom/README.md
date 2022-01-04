# Assignment 7 - The Great Firewall of Santa Cruz

This folder contains an executable that can read in
content from standard input, and determine if any
words that were stored in the Bloom filter and hash
table were present. These words are provided from two
supplied text files, oldspeak.txt and badspeak.txt

## Building

This folder contains a Makefile, which will build the
banhammer executable. To build the program, run:

$ make
$ make all
$ make banhammer

To clean the object files and executable run

$ make clean

## Running

This program can use command line arguments to set
certain details of the program to user specifications.

If done through command line, the following options are:

    -h          Program usage and help.
    -s          Print program statistics.
    -t size     Specify hash table size (default: 2^16).
    -f size     Specify Bloom filter size (default 2^20).

After the program is run, simply type whatever text you want
to be read, and a message will return if any of the words you
typed were flagged.

Credit to Professor Darrel Long and the Staff of CSE-13S

Enjoy!
~         
