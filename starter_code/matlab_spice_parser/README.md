How to compile and run my code! (under Ubuntu/ee-213/starter_code/matlab_spice_parser)

Step1	$ rm Symbol_Table.o main.o MNA_Matrix.o parse_func.o
(If you change any code under this folder, it's better to delete all .o files and complie again.)

Step2	$ make
(Compile)

Step3	$./runparse netlist_t1.sp
(netlist_t1.sp needs to be placed under /matlab_spice_parser)





Please also read ee213_final project_Lab record(实验记录)-Yibo Liu.pdf.  
This pdf records how to solve possible errors while compiling the project under ubuntu.











*****************************************************************************************************
README - a brief documentation on the package (student version)
March. 2018
by Guoyong SHI: shiguoyong@ic.sjtu.edu.cn

This circuit parser provides an interface to MATLAB so that students can
directly use MATLAB for circuit simulation.

This package contains the following files:
	Makefile
	parse.lex
	parse.y
	parse_func.h[.c]
	Symbol_Table.h[.c]
	MNA_Matrix.h[.c]
	main.c  -- the Linux OS driver

You should have installed CYGWIN (or a Linux equivalent) to
compile the flex-bison programs and run makefile.
You should also have installed a version of MATLAB on Windows or Linux.

The programs "parse.lex" and "parse.y" are compiled in CYGWIN by running
"make".

Then in MATLAB, run the mex compiler as follows:
	mex -DMATLAB mex_parse.c parse_func.c Symbol_Table.c MNA_Matrix.c

To run the parser in Linux, compile as follows:

	make parse
	make runparse

Depending on your need, the reader is expected to add more C-programs.

The basic funtionalities are

"parse_func.h[.c]" provide the parsing functions that
interfacing from the parser "parse.y" and the symbol tables;

"Symbol_Table.h[.c]" manages the symbol tables. 

"MNA_Matrix.h[.c]" manages the matrices resulting from parsing a netlist.

This package is incomplete and only provides a code skeleton for
continuing development.
The details on the necessary components will be introduced in the course.

For this project, please finish the following functions:
In MNA_Matrix.c

void Index_All_Nodes();
void Create_MNA_Matrix();



In Symbol_Table.c

void Init_Symbol_Tables();
void Delete_Node_Table();
void Delete_Device_Table();
void Delete_Node_Entry(const char *name);
Node_Entry* Lookup_Node_Entry(const char *name);
Device_Entry* Lookup_Device_Entry(const char *name);
Node_Entry* Insert_Node_Entry(const char *name);
Device_Entry* Insert_Device_Entry(const char *name,  const int numnodes, 
                 Node_Entry **nodelist, const double value);
void Print_Node_Table();
void Print_Device_Table();

For more information on this package, please consult TA or instructor.


