# Assignment 4 - The Perambulations of Denver Long

This program addresses the Traveling Salesman Problem, in which a path
through an adjacency matrix is created and returned, based on the edge
weights of the vertices.

## Building

This folder contains a Makefile, so just run
$ make all

## Running

To run this file, type:
$ ./tsp <args>

This program can use either command line arguments, or input the
details of the adjacency matrix through standard input.

If done through standard input, the input must follow this format:

    $ n number of vertices
    $ city_name_1
    $ city_name_2
    $ ...
    $ city_name_n
    $ x y value

Any number of edge weights can be inputted, but they must be within
bounds of the vertices involved. To end the input, press CTRL-D.
  
If done through command line arguments, the following options are:

    -u              Use undirected graph.

    -v              Enable verbose printing.

    -h              display program help and usage.

    -i infile       Input containing graph (default: stdin)

    -o outfile      Output of computed path (default: stdout)

If an unrecognized argument is detected, the program will default to
printing the help message, detailing program usage.

Note: in certain test cases, there may be memory leaks.

Enjoy!
        
