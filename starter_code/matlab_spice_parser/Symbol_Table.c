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
	NodeTable = malloc(sizeof(Node_Entry*));
	DeviceTable = malloc(sizeof(Device_Entry*));
	NodeTableSize = 0;
	DeviceTableSize = 0;

	memset(NodeTable, 0, sizeof(Node_Entry*));
	memset(DeviceTable, 0, sizeof(Device_Entry*));
}


void Destroy_Symbol_Table()
{
	Delete_Node_Table();
	Delete_Device_Table();
}

void Delete_Node_Table()
{
	Node_Entry *p = *NodeTable;
	while(p!=NULL){
		Node_Entry *cur = p;
		p = p->next;
		free(cur);
	}
	free(NodeTable);
}

void Delete_Device_Table()
{
	Device_Entry *p = *DeviceTable;
	while(p!=NULL){
		Device_Entry *cur = p;
		p = p->next;
		free(cur);
	}
	free(DeviceTable);
}

void Delete_Node_Entry(const char *name)
{
    Node_Entry *p = *NodeTable;
    if(!strcmp(p->name,name)){
        *NodeTable = (*NodeTable)->next;
        NodeTableSize--;
        return;
    }
    while(p!=NULL&&p->next!=NULL){
        if(!strcmp(p->next->name,name)){
            p->next = p->next->next;
            NodeTableSize--;
        }
        p = p->next;
    }
}

void Delete_Device_Entry(const char *name)
{
    Device_Entry *p = *DeviceTable;
    if(!strcmp(p->name,name)){
        *DeviceTable = (*DeviceTable)->next;
        DeviceTableSize--;
        return;
    }
    while(p!=NULL&&p->next!=NULL){
        if(!strcmp(p->next->name,name)){
            p->next = p->next->next;
            DeviceTableSize--;
        }
        p = p->next;
    }
}

Node_Entry* Lookup_Node_Entry(const char *name)
{
	Node_Entry* p = *NodeTable;
	while(p!=NULL){
		if(!strcmp(p->name,name)){
			return p;
		}
		p = p->next;
	}
	return NULL;
}


Device_Entry* Lookup_Device_Entry(const char *name)
{
	Device_Entry* p = *DeviceTable;
	while(p!=NULL){
		if(!strcmp(p->name,name)){
			return p;
		}
		p = p->next;
	}
	return NULL;
}


Node_Entry* Insert_Node_Entry(const char *name)
{
	//init
	Node_Entry* new_node = malloc(sizeof(Device_Entry));

	new_node->name = name;
	new_node->index = 0;

	//first insert new_node and then set the head to new_node
	new_node->next = *NodeTable;
	*NodeTable = new_node;
	NodeTableSize = NodeTableSize+1;

	return new_node;
}


Device_Entry* Insert_Device_Entry(const char *name,  const int numnodes, 
				Node_Entry **nodelist, const double value)
{
	//init
	Device_Entry* new_device = malloc(sizeof(Device_Entry));

	new_device->name = name;
	new_device->numnodes = numnodes;
	new_device->nodelist = nodelist;
	new_device->device = NULL;
	new_device->value = value;

	//first insert new_device and then set the head to new_device
	new_device->next = *DeviceTable;
	*DeviceTable = new_device;
	DeviceTableSize = DeviceTableSize+1;
	return new_device;
}


void Print_Node_Table()
{
	printf("\nNODE TABLE:\n");
	Node_Entry *p = *NodeTable;
	while(p!=NULL){
		printf("Node name: %s\n",p->name);
		printf("Node index: %d\n",p->index);
		p = p->next;
	}
}


void Print_Device_Table()
{
    printf("\nDEVICE TABLE:\n");
    Device_Entry *p = *DeviceTable;
    while(p!=NULL){
        printf("Device name: %s\n",p->name);
        printf("Device value: %f\n",p->value);
        printf("Device node number: %i\n",p->numnodes);
		int node_number = p->numnodes;
        for(int i=0;i<node_number;i++){
			printf("Node name: %s   ",p->nodelist[i]->name);
		}
		printf("\n");
		p = p->next;
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


