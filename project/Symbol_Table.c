/**
	Symbol_Table.c
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
// Electric type of a node variable
#define SP_NOTYPE 	0
#define SP_VOLTAGE 	1
#define SP_CURRENT 	2

#define LINE_INFO   "\n[" << __FILE__ << " : line " << __LINE__ << " : " << __FUNCTION__ << "()] - "
#define PRINT_LINE_INFO 	printf("\n[%s : line %d : %s()]\n", __FILE__, __LINE__, __FUNCTION__);


Node_Entry **NodeTable;
int NodeTableSize;

Device_Entry **DeviceTable;
int DeviceTableSize;

Node_Index **IndexTable;
int IndexTableSize;


void Init_Symbol_Tables()
{
	 NodeTable = malloc(sizeof(Node_Entry*));	 
	 NodeTableSize = 0;

	 DeviceTable = malloc(sizeof(Device_Entry*));	 
	 DeviceTableSize = 0;

	 IndexTable = malloc(sizeof(Node_Index*));	 
	 IndexTableSize = 0;
}


void Destroy_Symbol_Table()
{	
	//Delete_Node_Table();
	//Delete_Device_Table();
}


void Delete_Node_Table()
{   //not implemented in main
}

void Delete_Device_Table()
{   //not implemented in main
}

void Delete_Node_Entry(const char *name)
{//not implemented in main
}

void Delete_Device_Entry(const char *name)
{//not implemented in main
}

Node_Entry* Lookup_Node_Entry(const char *name)
{	
   Node_Entry *p = *NodeTable;
   int i;
   for(i=0; i<NodeTableSize; i++){
	  if(!strcmp(p->name, name)){	//if match found
		return p;}	
	  p=p->next;}
   return NULL;

}

Device_Entry* Lookup_Device_Entry(const char *name)
{	
   Device_Entry *p = *DeviceTable;
   int i;
   for(i=0; i<DeviceTableSize; i++){
	  if(!strcmp(p->name, name)){	//if match found
		return p;}	
	  p=p->next;}
   return NULL;


}

Node_Index* Lookup_Index_Entry(int hash)
{	
   Node_Index *p = *IndexTable;
   int i;
   for(i=0; i<IndexTableSize; i++){
	  if(p->hash_value == hash){	//if match found
		return p;}	
	  p=p->next;}
   return NULL;
}


Node_Entry* Insert_Node_Entry(const char *name)
{	
   Node_Entry *p = *NodeTable;
   Node_Entry *new = malloc(sizeof(Node_Entry));
   int i;
 
   //initialize new
   new->name = name;
   new->index = 0;
   new->node = NULL;	
   
   if(NodeTableSize==0) {
	*NodeTable = new;}
   else {
	for(i=0;i<NodeTableSize-1;i++)
		{p=p->next;
		 }
	p->next = new;} //inserting at the tail	
   NodeTableSize++;
   return new;    	

}


Device_Entry* Insert_Device_Entry(const char *name,  const int numnodes, 
				Node_Entry **nodelist, const double value)
{  int i;
   Device_Entry *p = *DeviceTable;
   Device_Entry *new = malloc(sizeof(Device_Entry));
 
   //initialize new
   new->name = name;
   new->numnodes = numnodes;
   new->nodelist= nodelist;
   new->value= value;	
   new->device = NULL;

   if(DeviceTableSize==0) {
	*DeviceTable = new;}
   else {
	for(i=0;i<DeviceTableSize-1;i++)
		{p=p->next;
		 }
	p->next = new;} //inserting at the tail	
   DeviceTableSize++;
   return new;    	
}

int Insert_Index_Entry(int hash)
{	
  Node_Index *p = Lookup_Index_Entry(hash); 
  
  if(p){
   return p->index;
   	   
}
else{
	Node_Index *new = malloc(sizeof(Node_Index));
 
   //initialize new
   new->hash_value = hash;
   new->index = IndexTableSize;
   new->next = *IndexTable;
   *IndexTable = new;
   IndexTableSize++;
   return new->index;
	}
}


void Print_Node_Table()
{	int i;
    Node_Entry *p = *NodeTable;	
    for(i=0;i< NodeTableSize; i++){
	printf("Node ::: ");
	printf("\nName = %s", p->name);
	printf("\tindex = %d\n", p->index); 
	p= p->next;
	}
}


void Print_Device_Table()
{	Device_Entry *p = *DeviceTable;	
	int i;
   	 for(i=0;i< DeviceTableSize; i++){
		printf("Device ::: %s", p->name);
		printf("\nNumber of nodes = %d", p->numnodes); 
		printf("\tValue = %f\n", p->value); 
		p= p->next;
	}
}


int NameHash(const char *name,  int tsize)
{
	const char *s; 
	register int i = 0; 

	// In case no name
	if (!name)	return (0);
	
	for (s = name; *s; s++) {
		i += (unsigned) *s; 
	}
	
	return (i % tsize);
}


