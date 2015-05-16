Chris Bubel
Michael McClosky

CISC 320 Programming Project 2, All Things Being Equal

This report is broken up into two parts, algorithm information and compiling
and running our program. Please read the section on compiling and running before
attempting to test the program.

ALGORITHM INFORMATION

General:

For our project we chose problem C from the ACM problem sheet. Instead
of approaching the problem as a series of linear systems, we stayed true to the
idea of searching and looked for a constant value before anything else. More
information about our balancing algorithm can be found below.

Thoughts on the Problem:

After thinking about the problem, we came to the conclusion that the only way
the output should be MANY is if all of the leaf nodes of the tree are variables.
As long as one finite weight is given, we can solve for all of the other nodes'
weights. In addition, the only way the solution should be NONE is if two of the
given finite weights do not compare as they should in the tree. For example, the
input [2 [ 2 A ] ] is incorrect because the given weights do not match up as
they should. The parent of any one node called current should have the weight
2 * current node's weight + 1. Likewise the child of a node should have weight
(current node's weight - 1) / 2. These insights made writing our algorithm much
easier.

The Algorithm:

The algorithm that we were posed to write is called balance in our source code.
This function contains a lot of conditional statements, and can be relatively
difficult to understand, so the general process is as follows:

  - Find a finite weight
  - Using that weight, pass values up and down the tree until all values are
    found
  - If the node is a variable and its value is found, write it out to an array
  - Print the contents of the node array.

The actual implementation is less simple than the points above, but that is the
process in a nutshell. Instead of solving a system of linear equations, we took
into account all of the possible cases and ways that a node could be reached. We
then wrote the correct behavior for all of those cases, and continued on with
our search for finding the weights.

Correctness:

We have been unable to find a valid input for which our algorithm does not find
the correct solution. Likewise, our algorithm has correctly determined whether a
given input has NONE or MANY solutions. Proving this correctness formally would
be very difficult.

Complexity:

From what we can see, the worst-case scenario for this algorithm would be that
all nodes except the rightmost are variables, and the rightmost is a correct
finite weight. If this is the case, then the following number of steps occurs:

  - n steps to get to the node all the way to the right
  - n steps to pass the weight from the finite node to all of the other nodes
  - log(n) steps to reach the top of the tree and then terminate

If our analysis is correct, then our function is O(n)

COMPILING AND RUNNING

Compiling the Program:

To compile the program, running the "make" command in terminal should suffice. If
you are on a unix-like machine, the make file might create two files, bal and
bal.exe, this is to be expected. On Windows, only bal.exe should be made. If the
make recipe fails, please enter "g++ -std=c++11 bal.cpp -o bal". Of course, you
can substitute compilers if you wish, but our function does make use of a C++11
function, so that must be supported. If you are on Windows append .exe to the
name of the output file in order to get an executable. If there appears to be a
problem with compilation, run "make clean", and then "make" again in order to
clean the program.

Running the Program:

On Windows, execute "bal.exe" in the command prompt.
On UNIX & UNIX-like machines execute "./bal" in terminal.

Our program reads in a text file called input.txt, each line of which contains a
test case. The file input.txt can be modified so more test cases provided by the
graders can be implemented. If the test cases span more than one line, the
program will most likely crash. Spaces are ignored when parsing input, as are
empty lines, so they should not affect the performance of the program.
