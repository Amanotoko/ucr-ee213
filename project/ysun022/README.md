README - a brief documentation on the package
Mar. 2016
by Yijing Sun

The program "parse.lex" and "parse.y" are compiled by running:

	make

To run the parser in Linux, compile as follows:

	make parse
	make runparse

Then to run the program run following command:

	./runparse inputfiilename

For example, input the example netlist, run:

	./runparse netlist_t1.sp

or

	./runparse rlc.sp