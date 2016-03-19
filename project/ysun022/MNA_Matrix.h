/**
	MNA_Matrix.h
	For the undergraduate EDA course offered at 
	Dept of Micro/Nano-electronics

	(c) Guoyong Shi, 
	All rights reserved, 2006-2015
	Shanghai Jiao Tong University

	If you modify the code, please keep the above copyright info.
*/

#ifndef MNA_MATRIX_H
#define MNA_MATRIX_H


#ifdef MATLAB
#include "mex.h"
#endif
#define nodenum  100

typedef struct Nodeindex
{
	char *name;	// node name to be allocated
	int Hash;	// Namehash of node name
	int index;	// Allocate index to node
	struct Nodeindex *next;
} Nodeindex;

extern int MatrixSize;	// size of the MNA matrix (i.e., the max dimension)
extern double **MNAMatrix;
extern double **MNAMatrix_image;
extern double *RHS;
extern int NodeSize;

void Create_MNA_Matrix();
int  Get_Matrix_Size();
void Get_MNA_System(double **A, double **b);
void Index_All_Nodes();
void Init_MNA_System();
void Print_MNA_System();


#endif
