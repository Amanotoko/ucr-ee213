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
#include "parse_func.h"
#include "MNA_Matrix.h"
#define WARNING_DIV_BY_ZERO \
		printf("\nWarning: divide by zero.");

int MatrixSize;	// size of the MNA matrix (i.e., the max dimension)
double **MNAMatrix = NULL;
double **MNAMatrix_image = NULL;
double *RHS = NULL;
int NodeSize = 0;
/**
	Assign indexes to all nodes in the node table.
	The ground node (with name "0") must be assigned index zero.
	The rest nodes are assigned indexes from 1, 2, 3 continuously.
*/
void Index_All_Nodes()
{
	Node_Entry *current; // the head of node is given as global varaiable
	NodeIndex *headIndex, *cmpIndex, *tailIndex;
	current = headNode;
	int index = 0;
	
	/*Initial NodeIndex*/
	headIndex=(NodeIndex *)malloc(sizeof(NodeIndex));
	headIndex->hash = -1;
	headIndex->name = NULL;
	headIndex->index = -1;
	headIndex->next = NULL;

	tailIndex=headIndex;
	while(current != tailNode){
		int same = 0;
		cmpIndex=headIndex->next;
		
		while(cmpIndex !=NULL){    //scan NodeIndex linked table
	    if(NameHash(current->next->name,nodenum) == cmpIndex->hash){   //compare hash
			if(strcmp(current->next->name,cmpIndex->name)==0){	//hash same, use exist index and continue
				current->next->index = cmpIndex->index;
				same=1;
			} 
		}
		cmpIndex=cmpIndex->next;
	}
		if(same==0){	// if new, assign index and add to NodeIndex link 
			current->next->index = index;
			NodeIndex *new;
			new=(NodeIndex *)malloc(sizeof(NodeIndex));
			new->name = current->next->name;
			new->hash = NameHash(current->next->name,nodenum);
			new->index = index;
			new->next = NULL;
			tailIndex->next=new;
			tailIndex=tailIndex->next;

			index++;
		}
	current = current->next;
	}
        NodeSize = index;
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
        MatrixSize = nVsrc+NodeSize; 

	MNAMatrix = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
	for (i = 0; i <= MatrixSize; i++) {
		MNAMatrix[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}

	MNAMatrix_image = (double**) malloc( (MatrixSize+1) * sizeof(double*) );
	for (i = 0; i <= MatrixSize; i++) {
		MNAMatrix_image[i] = (double*) malloc( (MatrixSize+1) * sizeof(double) );
	}

	RHS = (double*) malloc( (MatrixSize+1) * sizeof(double) );

	// Initialize to zero
	for (i = 0; i < MatrixSize; i++) {
		for (j = 0; j < MatrixSize; j++) {
			MNAMatrix[i][j] = 0.0;
			MNAMatrix_image[i][j]=0.0;
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
  Device_Entry *current;
  int i = 0;
  int j = 0;
  current = headRes->next;
  for (i = 0; i < nRes; i++){		
    MNAMatrix[current->nodelist->index][current->nodelist->index] += 1 / current->value;
    MNAMatrix[current->nodelist->next->index][current->nodelist->next->index] += 1 / current->value;
    MNAMatrix[current->nodelist->index][current->nodelist->next->index] += -1 / current->value;
    MNAMatrix[current->nodelist->next->index][current->nodelist->index] += -1 / current->value;
    current = current->next;
  }
  current = headCap->next;
  for (i = 0; i < nCap; i++){		
    MNAMatrix_image[current->nodelist->index][current->nodelist->index] += 1 / current->value;
    MNAMatrix_image[current->nodelist->next->index][current->nodelist->next->index] += 1 / current->value;
    MNAMatrix_image[current->nodelist->index][current->nodelist->next->index] += -1 / current->value;
    MNAMatrix_image[current->nodelist->next->index][current->nodelist->index] += -1 / current->value;
    current = current->next;
  }
  // current = headCapIC->next;
  // for (i = 0; i < nCapIC; i++){		
  //   MNAMatrix_image[current->nodelist->index][current->nodelist->index] += 1 / current->value;
  //   MNAMatrix_image[current->nodelist->next->index][current->nodelist->next->index] += 1 / current->value;
  //   MNAMatrix_image[current->nodelist->index][current->nodelist->next->index] += -1 / current->value;
  //   MNAMatrix_image[current->nodelist->next->index][current->nodelist->index] += -1 / current->value;
  //   current = current->next;
  // }
  current = headInd->next;
  for (i = 0; i < nInd; i++){		
    MNAMatrix_image[current->nodelist->index][current->nodelist->index] +=  current->value;
    MNAMatrix_image[current->nodelist->next->index][current->nodelist->next->index] +=  current->value;
    MNAMatrix_image[current->nodelist->index][current->nodelist->next->index] += -current->value;
    MNAMatrix_image[current->nodelist->next->index][current->nodelist->index] += -current->value;
    current = current->next;
  }
  // current = headIndIC;
  // for (i = 0; i < nCapIC; i++){		
  //   MNAMatrix_image[current->nodelist->index][current->nodelist->index] +=  current->value;
  //   MNAMatrix_image[current->nodelist->next->index][current->nodelist->next->index] +=  current->value;
  //   MNAMatrix_image[current->nodelist->index][current->nodelist->next->index] += -current->value;
  //   MNAMatrix_image[current->nodelist->next->index][current->nodelist->index] += -current->value;
  //   current = current->next;
  // }
  current = headVsrc->next;
  for (i = 0; i < nVsrc; i++){
    MNAMatrix[current->nodelist->index][NodeSize + i] += 1;
    MNAMatrix[current->nodelist->next->index][NodeSize + i] += -1;
    MNAMatrix[NodeSize + i][current->nodelist->next->index] += -1;
    MNAMatrix[NodeSize + i][current->nodelist->index] += 1;
    RHS[NodeSize + i] += current->value;
    current = current->next;
  }
  current = headIsrc->next;
  for (i = 0; i < nIsrc; i++){
    RHS[current->nodelist->index] += - current->value;
    RHS[current->nodelist->next->index] += current->value;
    current = current->next;
  }
  current = headVCCS->next;
  for (i = 0; i < nVCCS; i++){
    MNAMatrix[current->nodelist->index][current->nodelist->next->next->index] += current->value;
    MNAMatrix[current->nodelist->index][current->nodelist->next->next->next->index] += - current->value;
    MNAMatrix[current->nodelist->next->index][current->nodelist->next->next->index] += - current->value;
    MNAMatrix[current->nodelist->next->index][current->nodelist->next->next->next->index] += current->value;
    current = current->next;
  }
}

void Print_MNA_System()
{
	int i, j;

	printf("\nMNA Matrix:\n");
	for (j = 0; j <= MatrixSize; j++) {
		printf("\t%-24d", j);
	}
	printf("\tRHS\n");
	
	for (i = 0; i <= MatrixSize; i++) {
		printf("[%-3d]", i);
		for (j = 0; j <= MatrixSize; j++) {
			printf("\t%-6f+s*%-18f", MNAMatrix[i][j],MNAMatrix_image[i][j]);
		}
		printf("\t%-12f\n", RHS[i]);
	}
}


