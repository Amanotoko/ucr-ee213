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


void Init_Symbol_Tables()
{
	int node_entry_size=sizeof(Node_Entry*);
	int device_entry_size=sizeof(Device_Entry*);
	NodeTable =(Node_Entry **)malloc(node_entry_size);
	NodeTableSize = 0;
	DeviceTable =(Device_Entry **) malloc(device_entry_size);
	DeviceTableSize = 0;
}


void Destroy_Symbol_Table()
{
	Delete_Node_Table();
	Delete_Device_Table();
}


void Delete_Node_Table()	//not sure. need to delete every pointer in NodeTable too?
{
	free(NodeTable);
}

void Delete_Device_Table()
{
	 free(DeviceTable);
}

void Delete_Node_Entry(const char *name)
{
	Node_Entry *pre=*NodeTable;
	if(!strcmp(name,pre->name)){
        *NodeTable = (*NodeTable)->next;
        NodeTableSize=NodeTableSize-1;
        return;
    }
    Node_Entry *cur=pre->next;
	while(cur!=NULL){
    	if(!strcmp(name,cur->name)){
            pre->next = cur->next;
            NodeTableSize=NodeTableSize-1;
            break;
        }
        pre=cur;
        cur=cur->next;
    }
}

void Delete_Device_Entry(const char *name)
{
	// NOT IMPLEMENTED!
	Device_Entry *pre=*DeviceTable;
	if(!strcmp(name,pre->name)){
        *DeviceTable = (*DeviceTable)->next;
        DeviceTableSize=DeviceTableSize-1;
        return;
    }
    Device_Entry *cur=pre->next;
	while(cur!=NULL){
    	if(!strcmp(name,cur->name)){
            pre->next = cur->next;
            DeviceTableSize=DeviceTableSize-1;
            break;
        }
        pre=cur;
        cur=cur->next;
    }
}

Node_Entry* Lookup_Node_Entry(const char *name)
{
	Node_Entry* cur = *NodeTable;
	for (int i=0;i!=NodeTableSize;i++){
		if(!strcmp(name,cur->name)){
			return cur;
		}
		cur=cur->next;
	}
	return NULL;
}


Device_Entry* Lookup_Device_Entry(const char *name)
{

	Device_Entry* cur = *DeviceTable;
	for (int i=0;i!=DeviceTableSize;i++){
		if(!strcmp(name,cur->name)){
			return cur;
		}
		cur=cur->next;
	}
	return NULL;
}


Node_Entry* Insert_Node_Entry(const char *name)
{
	int node_entry_size=sizeof(Node_Entry);
	NodeTableSize++;
	Node_Entry* n;
	n=(Node_Entry* )malloc(node_entry_size);
	n->next=*NodeTable;
	*NodeTable = n;
	(*NodeTable)->name=(char*)name;
	(*NodeTable)->index=0;
	return *NodeTable;
}


Device_Entry* Insert_Device_Entry(const char *name,  const int numnodes, 
				Node_Entry **nodelist, const double value)
{
	int device_entry_size=sizeof(Device_Entry);
	DeviceTableSize++;
	Device_Entry* n;
	n=(Device_Entry*)malloc(device_entry_size);
	n->next=*DeviceTable;
	*DeviceTable = n;
	(*DeviceTable)->name=(char*)name;
	(*DeviceTable)->numnodes =(int)numnodes;
	(*DeviceTable)->nodelist = nodelist;
	(*DeviceTable)->value =(double)value;
	return *DeviceTable;
}


void Print_Node_Table()
{
	Node_Entry *cur = *NodeTable;
	printf("\nTotal nodes number=%d\n",NodeTableSize);
	for(int i=0;i!=NodeTableSize;i++){
		printf("Node%dname=%s\n",cur->index,cur->name);
		cur=cur->next;
	} 
}

void Print_Device_Table()
{
    Device_Entry *cur = *DeviceTable;
    printf("\nTotal device number=%d",DeviceTableSize);
    for(int i=0;i!=DeviceTableSize;i++){
    	printf("\nDevice=%s,value=%f,node_number=%d\nNodelist Detail:	",cur->name,cur->value,cur->numnodes);
        for(int ii=0;ii<cur->numnodes;ii++){
			printf("DeviceNode%d: %s, ",ii,cur->nodelist[ii]->name);
		}
		cur = cur->next;
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


