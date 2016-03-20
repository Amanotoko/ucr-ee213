To compile and run the program, these steps are to be followed, after navigating to your working directory on linux:

make parse
make runparse
./runparse netlist_t1.sp
./runparse rlc.sp

Here make parse and runparse creates 2 files for compiling the code.

The source files where changes have been made are:

1. Parse_func.c : Implemented the code to parse every device 

2. Symbol_Table.c : Implemented the code to initialize and print the symbol table with  components:
* Node Table
* Device Table
* Index Tsble

3.MNA_Matrix: Initially all the nodes are indexed, and then the MNA Matrix is created for ll the nodes present.
