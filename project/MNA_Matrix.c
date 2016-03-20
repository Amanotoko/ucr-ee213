/**
	MNA_Matrix.c
	For the undergraduate EDA course offered at 
	Dept of Micro/Nano-electronics

	(c) Guoyong Shi, 
	All rights reserved, 2006-2015
	Shanghai Jiao Tong University

	If you modify the code, please keep the above copyright info.
*/

#ifdef MATLAB
#include "mex.h"
#endif

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "Symbol_Table.h"
#include "MNA_Matrix.h"


#define WARNING_DIV_BY_ZERO \
		printf("\nWarning: divide by zero.");

int MatrixSize = 0;	// size of the MNA matrix (i.e., the max dimension)
double **MNAMatrix = NULL;
double **MNAMatrixReactive = NULL;
double *RHS = NULL;

/**
	Assign indexes to all nodes in the node table.
	The ground node (with name "0") must be assigned index zero.
	The rest nodes are assigned indexes from 1, 2, 3 continuously.
*/
void Index_All_Nodes()
{		
	Node_Entry *cur= *NodeTable; //to traverse through the node table
	int i;
	//first, initializing the ground node to 0:
	Insert_Index_Entry(0);
	for(i=0; i<NodeTableSize; i++){
	   int h= NameHash(cur->name, NodeTableSize);
	   int in= Insert_Index_Entry(h);           
	   cur->index= in;	//setting the index for each node
	   cur= cur->next;   	//moving to the next node
	}
	
}

int Get_Matrix_Size()
{
	return MatrixSize;
}

/**
	The MATLAB matrix data are stored in column-major format.
*/
void Get_MNA_System(double **A, double **b)
{
	int i, j;

	for (j = 0; j <= MatrixSize; j++) {
		for (i = 0; i <= MatrixSize; i++) {
			(*A)[i+j*(MatrixSize+1)] = MNAMatrix[i][j];  // convert to column-major format
		}
		(*b)[j] = RHS[j];
	}
}


/**
	Create memory for MNAMatrix and RHS.
	The adopted storage is in DENSE matrix format.
	The zeroth row/col correspond to the ground.

	Must call Index_All_Nodes() before calling this function.

	(!!) Students can try SPARSE matrix format.
*/
void Init_MNA_System()
{
#if 1
	int i, j;
	
	MatrixSize= IndexTableSize - 1;	

	MNAMatrix = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
	for (i = 0; i <= MatrixSize; i++) {
		MNAMatrix[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}
	
	//MNA Matrix for reactive components
	MNAMatrixReactive = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
	for (i = 0; i <= MatrixSize; i++) {
		MNAMatrixReactive[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}

	RHS = (double*) malloc( (MatrixSize+1) * sizeof(double) );

	// Initialize to zero
	for (i = 0; i <= MatrixSize; i++) {
		for (j = 0; j <= MatrixSize; j++) {
			MNAMatrix[i][j] = 0.0;
			MNAMatrixReactive[i][j] = 0.0;
		}
		RHS[i] = 0.0;
	}
#endif
}

/**
	Create the MNA matrix by scanning the device table.
	In principle stamping of each element should be done by the device instance.
	In this example code we do not create device objects. 
	Therefore, stamping is done inside this function for simplicity.
	
	(!!) Students are required to develop device classes to fulfil this task.
*/
void Create_MNA_Matrix()
{	
	int i;
	Device_Entry *d= *DeviceTable;	//creating temporary device pointer
	for(i=0; i<DeviceTableSize; i++){
	//When device= Resistor	
	if(d->name[0] == 'R'){
		MNAMatrix[d->nodelist[0]->index][d->nodelist[0]->index] +=  1/ d->value;
		MNAMatrix[d->nodelist[1]->index][d->nodelist[0]->index] +=  -1/ d->value;
		MNAMatrix[d->nodelist[0]->index][d->nodelist[1]->index] +=  -1/ d->value;
		MNAMatrix[d->nodelist[1]->index][d->nodelist[1]->index] +=  1/ d->value;
		d=d->next;
		}

	//When device= Capacitor	
	else if(d->name[0] == 'C'){
		MNAMatrixReactive[d->nodelist[0]->index][d->nodelist[0]->index] +=  1/ d->value;
		MNAMatrixReactive[d->nodelist[1]->index][d->nodelist[0]->index] +=  -1/ d->value;
		MNAMatrixReactive[d->nodelist[0]->index][d->nodelist[1]->index] +=  -1/ d->value;
		MNAMatrixReactive[d->nodelist[1]->index][d->nodelist[1]->index] +=  1/ d->value;
		d=d->next;
		}

	//When device= Inductor	
		
	else if(d->name[0] == 'L'){
		MNAMatrixReactive[d->nodelist[0]->index][d->nodelist[0]->index] +=  d->value;
		MNAMatrixReactive[d->nodelist[1]->next->index][d->nodelist[0]->index] +=  d->value;
		MNAMatrixReactive[d->nodelist[0]->index][d->nodelist[1]->index] +=  -d->value;
		MNAMatrixReactive[d->nodelist[1]->index][d->nodelist[1]->index] += -d->value;
		d=d->next;
		}

	//When device= Isrc	
	else if(d->name[0] == 'I'){
		//right hand side values stored
		RHS[d->nodelist[0]->index] += -d->value;
		RHS[d->nodelist[1]->index] += d->value;	
		d=d->next;	
		}

	//When device= VCCS	
	else if(d->name[0] == 'G'){
		MNAMatrix[d->nodelist[0]->index][d->nodelist[2]->index] += d->value;
		MNAMatrix[d->nodelist[1]->index][d->nodelist[3]->index] +=  d->value;
		MNAMatrix[d->nodelist[0]->index][d->nodelist[3]->index] +=  -d->value;
		MNAMatrix[d->nodelist[1]->index][d->nodelist[2]->index] += -d->value;
		d=d->next;
		}

	}
}

void Print_MNA_System()
{
	int i, j;

	printf("\n-----MNA Matrix-----\n");
	for (j = 0; j <= MatrixSize; j++) {
		printf("\t%-24d", j);
	}
	printf("\tRHS\n");
	
	for (i = 0; i <= MatrixSize; i++) {
		printf("\n[%-3d]", i);
		for (j = 0; j <= MatrixSize; j++) {
			printf("\t%-6f+s*%-18f", MNAMatrix[i][j],MNAMatrixReactive[i][j]);
		}
		printf("\t%-12f\n", RHS[i]);
	}
}


