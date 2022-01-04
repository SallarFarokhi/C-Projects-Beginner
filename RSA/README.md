# Assignment 6 - Public Key Cryptography

This folder contains three seperate executables 
that can encrypt a files contents, decrypt it,
or generate public and private keys, and write
out to a specified file location. This is done
through RSA encryption. If a file location is 
not specified, then it will read in from 
standard input and output.

## Building

This folder contains a Makefile, which is capable of
building three different executables, keygen, encrypt,
and decrypt.
To build everything, run:
$ make all
To build the key generator, run:
$ make keygen
To build the encryptor, run:
$ make encrypt
To build the decryptor, run:
$ make decrypt

## Running

This program can either use command line arguments, or input
the details of the programs files through standard input.

If done through standard input, simply input the name of the
file into the terminal.

If done through command line arguments, the following options are

    -h          Program Usage and help.
    -v          Print compression statistics.
    -i infile   Input file (default: stdin).
    -o outfile  Output file (default: stdout).
    -p pbfile   Public key file (default: rsa.pub)
    -n pvfile   Private key file (default: rsa.priv)

If an unrecognized argument is detected, the program will default to
printing the help message, detailing program usage.

Credit to Professor Darrel Long and the Staff of CSE-13S

Enjoy!
