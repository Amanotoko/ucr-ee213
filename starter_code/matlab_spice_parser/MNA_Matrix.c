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
int size_na = 0;  // size of the na matrix
int size_m = 0;  // size of the m matrix
double **MNAMatrix = NULL;
double **MNAMatrix1 = NULL; // add another matrix to record the inductor and conductor
double *RHS = NULL;

/**
	Assign indexes to all nodes in the node table.
	The ground node (with name "0") must be assigned index zero.
	The rest nodes are assigned indexes from 1, 2, 3 continuously.
*/
void Index_All_Nodes()
{
    int size=0;
    Node_Entry *p = *NodeTable;
    while(p != NULL){
        p->index = NameHash(p->name,NodeTableSize);
        if(p->index > size){
            size = p->index;
        }
        p = p->next;
    }
    size_na = size;
    size_m = nVsrc + nInd;
    MatrixSize = size_m+size_na;
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
	if (MatrixSize == 0) {
		return;
	}

	MNAMatrix = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
	for (i = 0; i <= MatrixSize; i++) {
		MNAMatrix[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}

    MNAMatrix1 = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
    for (i = 0; i <= MatrixSize; i++) {
        MNAMatrix1[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
    }

	RHS = (double*) malloc( (MatrixSize+1) * sizeof(double) );

	// Initialize to zero
	for (i = 0; i <= MatrixSize; i++) {
		for (j = 0; j <= MatrixSize; j++) {
			MNAMatrix[i][j] = 0.0;
            MNAMatrix1[i][j] = 0.0;
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
    Device_Entry *p = *DeviceTable;
    while(p != NULL){
        char device = p->name[0];
        if(device=='R') {
            MNAMatrix[p->nodelist[0]->index][p->nodelist[0]->index] += 1.0 / p->value;
            MNAMatrix[p->nodelist[1]->index][p->nodelist[1]->index] += 1.0 / p->value;
            MNAMatrix[p->nodelist[0]->index][p->nodelist[1]->index] -= 1.0 / p->value;
            MNAMatrix[p->nodelist[1]->index][p->nodelist[0]->index] -= 1.0 / p->value;
        }
        else if(device=='C') {
            MNAMatrix1[p->nodelist[0]->index][p->nodelist[0]->index] += p->value;
            MNAMatrix1[p->nodelist[1]->index][p->nodelist[1]->index] += p->value;
            MNAMatrix1[p->nodelist[0]->index][p->nodelist[1]->index] -= p->value;
            MNAMatrix1[p->nodelist[1]->index][p->nodelist[0]->index] -= p->value;
        }
        else if(device=='L'){
            // will put the element in the biggest na+1
            size_na = size_na+1;
            MNAMatrix[p->nodelist[0]->index][size_na] = 1;
            MNAMatrix[p->nodelist[1]->index][size_na] = -1;
            MNAMatrix[size_na][p->nodelist[0]->index] = 1;
            MNAMatrix[size_na][p->nodelist[1]->index] = -1;
            MNAMatrix1[size_na][size_na] -= p->value;
        }
        else if(device=='G') {
            MNAMatrix[p->nodelist[0]->index][p->nodelist[2]->index] += p->value;
            MNAMatrix[p->nodelist[1]->index][p->nodelist[2]->index] -= p->value;
            MNAMatrix[p->nodelist[0]->index][p->nodelist[3]->index] -= p->value;
            MNAMatrix[p->nodelist[1]->index][p->nodelist[3]->index] += p->value;
        }
        else if(device=='I') {
            RHS[p->nodelist[0]->index] -= p->value;
            RHS[p->nodelist[1]->index] += p->value;
        }
        else if(device=='V'){
            // will put the element in the biggest na+1
            size_na = size_na+1;
            MNAMatrix[p->nodelist[0]->index][size_na] = 1;
            MNAMatrix[p->nodelist[1]->index][size_na] = -1;
            MNAMatrix[size_na][p->nodelist[0]->index] = 1;
            MNAMatrix[size_na][p->nodelist[1]->index] = -1;
            RHS[size_na] += p->value;
        }
        p = p->next;
    }
}

void Print_MNA_System()
{
	int i, j;

    FILE *f;
    f = fopen("MNAmatrix.txt", "w");

	printf("\n\n\t\t\t\t\t\t\tMNA Matrix:\n");
	for (j = 0; j <= MatrixSize; j++) {
		printf("\t%-24d", j);
	}
	printf("\tRHS");
	for (i = 0; i <= MatrixSize; i++) {
		printf("\n[%-3d]", i);
		for (j = 0; j <= MatrixSize; j++) {
            printf("\t%-6f+s*%-18f", MNAMatrix[i][j],MNAMatrix1[i][j]);
		}
		printf("\t%-12f", RHS[i]);
	}

    for (i = 0; i <= MatrixSize; i++) {
        for (j = 0; j <= MatrixSize; j++) {
            fprintf(f,"\t%f", MNAMatrix[i][j]);
        }
    }
    fprintf(f,"\n");
    for (i = 0; i <= MatrixSize; i++) {
        fprintf(f,"\t%f", RHS[i]);
    }
    fclose(f);
}


