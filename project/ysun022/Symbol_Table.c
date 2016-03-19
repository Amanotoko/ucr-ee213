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
#include "parse_func.h"

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
    Reshead=(struct device_s *) malloc(sizeof(struct device_s));
    Reshead->next=NULL;
    Restail=Reshead;
    Caphead=(struct device_s *) malloc(sizeof(struct device_s));
    Caphead->next=NULL;
    Captail=Caphead;

    CapIChead=(struct device_s *) malloc(sizeof(struct device_s));
    CapIChead->next=NULL;
    CapICtail=CapIChead;

    Indhead=(struct device_s *) malloc(sizeof(struct device_s));
    Indhead->next=NULL;
    Indtail=Indhead;

    IndIChead=(struct device_s *) malloc(sizeof(struct device_s));
    IndIChead->next=NULL;
    IndICtail=IndIChead;

    Vsrchead=(struct device_s *) malloc(sizeof(struct device_s));
    Vsrchead->next=NULL;
    Vsrctail=Vsrchead;

    Isrchead=(struct device_s *) malloc(sizeof(struct device_s));
    Isrchead->next=NULL;
    Isrctail=Isrchead;

    VCCShead=(struct device_s *) malloc(sizeof(struct device_s));
    VCCShead->next=NULL;
    VCCStail=VCCShead;

    Nodehead=(struct node_s*) malloc(sizeof(struct node_s*) );
    Nodetail=Nodehead;
    Nodehead->next=NULL;
}


void Destroy_Symbol_Table()
{
	Delete_Node_Table();
	Delete_Device_Table();
}


void Delete_Node_Table()
{
}

void Delete_Device_Table()
{
}

void Delete_Node_Entry(const char *name)
{
}

void Delete_Device_Entry(const char *name)
{
	// NOT IMPLEMENTED!
}

Node_Entry* Lookup_Node_Entry(const char *name)
{
	return  NULL;
}


Device_Entry* Lookup_Device_Entry(const char *name)
{
	return NULL;
}

Node_Entry* Insert_Node_Entry(const char *name)
{
	return NULL;
}

Device_Entry* Insert_Device_Entry(const char *name,  const int numnodes, 
				Node_Entry **nodelist, const double value)
{
	return NULL;
}

void Print_Node_Table()
{
}

/*
void Print_Device_Table()
{
    Device_Entry *Print[]={Restail,Caphead,Indhead,Vsrchead,Isrchead,VCCShead};
    for(int i=0; i<5;i++)
    {
	do
	{
		printf("%s, %d, %d, %d, %g\n", Print[i]->next->name,Print[i]->next->numnodes,*(Print[i]->next->nodelist)->index,*(Print[i]->next->nodelist)->next->index,Print[i]->value);
		Print[i]=Print[i]->next;
	} while (Print[i]!=NULL);
    }
    while (Print[5])
    {
	printf("%s, %d, %d, %d, %d, %d\n", Print[i]->next->name,Print[i]->next->numnodes,*(Print[i]->next->nodelist)->index,*(Print[i]->next->nodelist)->next->index,*(Print[i]->next->nodelist)->next->next->index,*(Print[i]->next->nodelist)->next->next->next->index,Print[i]->value);
    }
}
*/

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
