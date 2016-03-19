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
	Node_Entry *p; // Nodehead should be given as global parameter
	Nodeindex *Indexhead, *q, *q2;
	p = Nodehead;
	int index = 0;
	
	/*Initial Nodeindex*/
	Indexhead=(Nodeindex *)malloc(sizeof(Nodeindex));
	Indexhead->Hash = -1;
	Indexhead->name = NULL;
	Indexhead->index = -1;
	Indexhead->next = NULL;

	q2=Indexhead;
	while(p != Nodetail){
		int flag = 0;
		q=Indexhead->next;
		
		while(q !=NULL){    //Scan Nodeindex link table
	    if(NameHash(p->next->name,nodenum) == q->Hash){   //Compare Hash
			if(strcmp(p->next->name,q->name)==0){
				p->next->index = q->index;flag=1;
			} 
			//If name is same, use allocated index, break
		}
		q=q->next;
	}
		if(flag==0){	// if is new name, allocate index and add to Nodeindex link table. 
			p->next->index = index;
			Nodeindex *newnode;
			newnode=(Nodeindex *)malloc(sizeof(Nodeindex));
			newnode->name = p->next->name;
			newnode->Hash = NameHash(p->next->name,nodenum);
			newnode->index = index;
			newnode->next = NULL;
			q2->next=newnode;
			q2=q2->next;

			index++;
		}
	p = p->next;
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
  Device_Entry *temp;
  temp = Reshead->next;
  int i = 0;
  int j = 0;
  //printf("nRes%d\n", nRes);
  for (i = 0; i < nRes; i++){		
    MNAMatrix[temp->nodelist->index][temp->nodelist->index] += 1 / temp->value;
    MNAMatrix[temp->nodelist->next->index][temp->nodelist->next->index] += 1 / temp->value;
    MNAMatrix[temp->nodelist->index][temp->nodelist->next->index] += -1 / temp->value;
    MNAMatrix[temp->nodelist->next->index][temp->nodelist->index] += -1 / temp->value;
    temp = temp->next;
  }
// for (i = 0; i <MatrixSize; i++) {
// 		for (j = 0; j < MatrixSize; j++) {
			
// 			printf("%f\t",MNAMatrix[i][j]);

// 		}printf("\n");
// 	}
	// printf("%d %d\n %d %d\n", MNAMatrix[temp->nodelist->index][temp->nodelist->index] ,MNAMatrix[temp->nodelist->index][temp->nodelist->next->index] , MNAMatrix[temp->nodelist->next->index][temp->nodelist->index], MNAMatrix[temp->nodelist->next->index][temp->nodelist->next->index]);
  temp = Caphead->next;
  for (i = 0; i < nCap; i++){		
    MNAMatrix_image[temp->nodelist->index][temp->nodelist->index] += 1 / temp->value;
    MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->next->index] += 1 / temp->value;
    MNAMatrix_image[temp->nodelist->index][temp->nodelist->next->index] += -1 / temp->value;
    MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->index] += -1 / temp->value;
    temp = temp->next;
  }
  // for (i = 0; i < nCapIC; i++){		
  //   MNAMatrix_image[temp->nodelist->index][temp->nodelist->index] += 1 / temp->value;
  //   MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->next->index] += 1 / temp->value;
  //   MNAMatrix_image[temp->nodelist->index][temp->nodelist->next->index] += -1 / temp->value;
  //   MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->index] += -1 / temp->value;
  // 	temp = temp->next;
  // }
  temp = Indhead->next;
  for (i = 0; i < nInd; i++){		
    MNAMatrix_image[temp->nodelist->index][temp->nodelist->index] +=  temp->value;
    MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->next->index] +=  temp->value;
    MNAMatrix_image[temp->nodelist->index][temp->nodelist->next->index] += -temp->value;
    MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->index] += -temp->value;
    temp = temp->next;
  }
  // temp = IndIChead;
  // 	for (i = 0; i < nCapIC; i++){		
  //   MNAMatrix_image[temp->nodelist->index][temp->nodelist->index] +=  temp->value;
  //   MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->next->index] +=  temp->value;
  //   MNAMatrix_image[temp->nodelist->index][temp->nodelist->next->index] += -temp->value;
  //   MNAMatrix_image[temp->nodelist->next->index][temp->nodelist->index] += -temp->value;
  // 	temp = temp->next;
  // }
  temp = Vsrchead->next;
  for (i = 0; i < nVsrc; i++){
    MNAMatrix[temp->nodelist->index][NodeSize + i] += 1;
    MNAMatrix[temp->nodelist->next->index][NodeSize + i] += -1;
    MNAMatrix[NodeSize + i][temp->nodelist->next->index] += -1;
    MNAMatrix[NodeSize + i][temp->nodelist->index] += 1;
    RHS[NodeSize + i] += temp->value;
    temp = temp->next;
  }
  temp = Isrchead->next;
  for (i = 0; i < nIsrc; i++){
    RHS[temp->nodelist->index] += - temp->value;
    RHS[temp->nodelist->next->index] += temp->value;
    temp = temp->next;
  }
  temp = VCCShead->next;
  for (i = 0; i < nVCCS; i++){
    MNAMatrix[temp->nodelist->index][temp->nodelist->next->next->index] += temp->value;
    MNAMatrix[temp->nodelist->index][temp->nodelist->next->next->next->index] += - temp->value;
    MNAMatrix[temp->nodelist->next->index][temp->nodelist->next->next->index] += - temp->value;
    MNAMatrix[temp->nodelist->next->index][temp->nodelist->next->next->next->index] += temp->value;
    temp = temp->next;
  }
}
//  for (i = 0; i <MatrixSize; i++) {
// 		for (j = 0; j < MatrixSize; j++) {
			
// 			printf("%f\t",MNAMatrix[i][j]);

// 		}printf("\n");
// 	}
// 	printf("\n");
// 	 for (i = 0; i <MatrixSize; i++) {
// 		for (j = 0; j < MatrixSize; j++) {
			
// 			printf("%f\t",MNAMatrix_image[i][j]);

// 		}printf("\n");
// 	}
// }

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


