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
#include "parse_func.h"

#define WARNING_DIV_BY_ZERO \
		printf("\nWarning: divide by zero.");

int MatrixSize = 0;	// size of the MNA matrix (i.e., the max dimension)
double **MNAMatrix = NULL;
double *RHS = NULL;

/**
	Assign indexes to all nodes in the node table.
	The ground node (with name "0") must be assigned index zero.
	The rest nodes are assigned indexes from 1, 2, 3 continuously.
*/
void Index_All_Nodes()
{
	Node_Entry *n = *NodeTable;
	for(int i=0;i!=NodeTableSize;i++){
		n->index = NameHash(n->name,NodeTableSize);
		n=n->next;
	}
	Node_Entry *cur = *NodeTable;
	int common_node_size=0;
	while(cur != NULL){
		if(cur->index>common_node_size)
			common_node_size=cur->index;
		cur=cur->next;
	}
	MatrixSize=common_node_size+nInd+nVsrc;
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
double **MNA_C_Matrix = NULL;
void Init_MNA_System()
{
#if 1
	int i, j;
	
	if (MatrixSize == 0) {
		return;
	}

	MNAMatrix = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
	for (i = 0; i <= MatrixSize; i++) {
		MNAMatrix[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}

	RHS = (double*) malloc( (MatrixSize+1) * sizeof(double) );

	// Initialize to zero
	for (i = 0; i <= MatrixSize; i++) {
		for (j = 0; j <= MatrixSize; j++) {
			MNAMatrix[i][j] = 0.0;
		}
		RHS[i] = 0.0;
	}

	if (MatrixSize == 0) {
		return;
	}

	MNA_C_Matrix = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
	for (i = 0; i <= MatrixSize; i++) {
		MNA_C_Matrix[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}
	for (i = 0; i <= MatrixSize; i++) {
		for (j = 0; j <= MatrixSize; j++) {
			MNA_C_Matrix[i][j] = 0.0;
		}
	}

	if (MatrixSize == 0) {
		return;
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
	Device_Entry *n = *DeviceTable;
    while(n!= NULL){
		switch(n->name[0]){
        	case 'R':
                MNAMatrix[n->nodelist[0]->index][n->nodelist[0]->index] += 1.0/(n->value);
                MNAMatrix[n->nodelist[1]->index][n->nodelist[1]->index] += 1.0/(n->value);
                MNAMatrix[n->nodelist[0]->index][n->nodelist[1]->index] -= 1.0/(n->value);
                MNAMatrix[n->nodelist[1]->index][n->nodelist[0]->index] -= 1.0/(n->value);break;
            
            case 'G':   //voltage source
                MNAMatrix[n->nodelist[0]->index][n->nodelist[2]->index] += (n->value);
                MNAMatrix[n->nodelist[0]->index][n->nodelist[3]->index] -= (n->value);
                MNAMatrix[n->nodelist[1]->index][n->nodelist[2]->index] -= (n->value);
                MNAMatrix[n->nodelist[1]->index][n->nodelist[3]->index] += (n->value);break;
            

            
            case 'C':   //capacitor
                MNA_C_Matrix[n->nodelist[0]->index][n->nodelist[0]->index] += 1.0/(n->value);
                MNA_C_Matrix[n->nodelist[1]->index][n->nodelist[1]->index] += 1.0/(n->value);
                MNA_C_Matrix[n->nodelist[0]->index][n->nodelist[1]->index] -= 1.0/(n->value);
                MNA_C_Matrix[n->nodelist[1]->index][n->nodelist[0]->index] -= 1.0/(n->value);break;

            case 'L': 
                MNA_C_Matrix[n->nodelist[0]->index][n->nodelist[0]->index] += (n->value);
                MNA_C_Matrix[n->nodelist[1]->index][n->nodelist[1]->index] += (n->value);
                MNA_C_Matrix[n->nodelist[0]->index][n->nodelist[1]->index] -= (n->value);
                MNA_C_Matrix[n->nodelist[1]->index][n->nodelist[0]->index] -= (n->value);break;
            case 'I': //current source
                RHS[n->nodelist[0]->index] -= n->value;
                RHS[n->nodelist[1]->index] += n->value; break;

        };

        n = n->next;
    }
}

void Print_MNA_System()
{
	int i, j;

	printf("\n\n");
	for (j = 0; j <= MatrixSize; j++) {
		printf("\t%-18d", j);
	}
	printf("\tRHS");
	
	for (i = 0; i <= MatrixSize; i++) {
		printf("\n[%-3d]", i);
		for (j = 0; j <= MatrixSize; j++) {
			printf("\t%-3f+%-3fs", MNAMatrix[i][j],MNA_C_Matrix[i][j]);
		}
		printf("\t%-12f", RHS[i]);
	}
}


