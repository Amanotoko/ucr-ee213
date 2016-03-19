/************************************************************
  	parse_func.c
  	containing parsing functions for SPICE netlist parser example.

Author(s):
   	Shi, Guoyong  2015
 
	Course material for "Introduction to EDA", 2015
   	Dept of Micro/Nano-electronics
   	Shanghai Jiao Tong University (SJTU)
************************************************************/

#ifdef MATLAB
#include "mex.h"
#endif

#include "parse_func.h"
#include "Symbol_Table.h"

// Global variables defined for the parser utility functions
int nRes;
int nCap;
int nInd;
int nVsrc;
int nIsrc;
int nVCCS;
Device_Entry **DeviceTable;

void Init_parse_util()
{
	nRes = 0;
	nCap = 0;
	nInd = 0;
	nVsrc = 0;
	nIsrc = 0;
	nVCCS = 0;
}

void ParseRes(char *name, char *node1, char *node2, double value)
{
	int numnodes;
	Node_Entry *nodelist;
	Device_Entry *deviceRes;

	printf("[Resistor parsed ...]\n");
	printf("   name=%s, node+=%s, node-=%s, R=%e\n", name, node1, node2, value);
	nRes++;

	deviceRes = (struct device_s *) malloc(sizeof(struct device_s));
	deviceRes->name=name;
	deviceRes->numnodes=2;
	Node_Entry *N1=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N2=(struct node_s*) malloc(sizeof(struct node_s*) );
	N1->name=node1;
	N2->name=node2;
	N1->next=N2;
	N2->next=NULL;
	nodelist=N1;
        deviceRes->nodelist = nodelist;
	deviceRes->value=value;
	deviceRes->next=NULL;

	Restail->next=deviceRes;
	Restail=deviceRes;

	Nodetail->next=N1;
	Nodetail=N2;
}

void ParseCap(char *name, char *node1, char *node2, double value)
{
	int numnodes;
	Node_Entry *nodelist;
	Device_Entry *deviceCap;
	
	printf("[Capacitor parsed ...]\n");
	printf("   name=%s, node+=%s, node-=%s, C=%e\n", name, node1, node2, value);
	nCap++;
	deviceCap = (struct device_s *) malloc(sizeof(struct device_s));
	deviceCap->name=name;
	deviceCap->numnodes=2;
	Node_Entry *N1=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N2=(struct node_s*) malloc(sizeof(struct node_s*) );
	N1->name=node1;
	N2->name=node2;
	N1->next=N2;
	N2->next=NULL;
	nodelist=N1;
        deviceCap->nodelist = nodelist;
	deviceCap->value=value;
	deviceCap->next=NULL;

	Captail->next=deviceCap;
	Captail=deviceCap;

	Nodetail->next=N1;
	Nodetail=N2;
}

void ParseCapIC(char *name, char *node1, char *node2, double value, double init)
{
	printf("[Capacitor parsed ...]\n");
	printf("   name=%s, node+=%s, node-=%s, C=%e, init=%e\n", name, node1, node2, value, init);
	nCap++;

	// Save the device, nodes, value info to the symbol tables.
	// Please write your own code here ...
}

void ParseInd(char *name, char *node1, char *node2, double value)
{
	int numnodes;
	Node_Entry *nodelist;
	Device_Entry *deviceInd;
	
	printf("[Inductor parsed ...]\n");
	printf("   name=%s, node+=%s, node-=%s, L=%e\n", name, node1, node2, value);
	nInd++;
	deviceInd = (struct device_s *) malloc(sizeof(struct device_s));
	deviceInd->name=name;
	deviceInd->numnodes=2;
	Node_Entry *N1=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N2=(struct node_s*) malloc(sizeof(struct node_s*) );
	N1->name=node1;
	N2->name=node2;
	N1->next=N2;
	N2->next=NULL;
	nodelist=N1;
        deviceInd->nodelist = nodelist;
	deviceInd->value=value;
	deviceInd->next=NULL;

	Indtail->next=deviceInd;
	Indtail=deviceInd;

	Nodetail->next=N1;
	Nodetail=N2;
}

void ParseIndIC(char *name, char *node1, char *node2, double value, double init)
{
	printf("[Inductor parsed ...]\n");
	printf("   name=%s, node+=%s, node-=%s, L=%e, init=%e\n", name, node1, node2, value, init);
	nInd++;

	// Save the device, nodes, value info to the symbol tables.
	// Please write your own code here ...
}

void ParseVsrc(char *name, char *node1, char *node2, double value)
{
	int numnodes;
	Node_Entry *nodelist;
	Device_Entry *deviceVsrc;
	
	printf("[Voltage Source parsed ...]\n");
	printf("   name=%s, node+=%s, node-=%s, V=%e\n", name, node1, node2, value);
	nVsrc++;
	deviceVsrc = (struct device_s *) malloc(sizeof(struct device_s));
	deviceVsrc->name=name;
	deviceVsrc->numnodes=2;
	Node_Entry *N1=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N2=(struct node_s*) malloc(sizeof(struct node_s*) );
	N1->name=node1;
	N2->name=node2;
	N1->next=N2;
	N2->next=NULL;
	nodelist=N1;
        deviceVsrc->nodelist = nodelist;
	deviceVsrc->value=value;
	deviceVsrc->next=NULL;

	Vsrctail->next=deviceVsrc;
	Vsrctail=deviceVsrc;

	Nodetail->next=N1;
	Nodetail=N2;
}

void ParseIsrc(char *name, char *node1, char *node2, double value)
{
	int numnodes;
	Node_Entry *nodelist;
	Device_Entry *deviceIsrc;
	
	printf("[Current source parsed ...]\n");
	printf("   name=%s, node+=%s, node-=%s, I=%e\n", name, node1, node2, value);
	nIsrc++;
	deviceIsrc = (struct device_s *) malloc(sizeof(struct device_s));
	deviceIsrc->name=name;
	deviceIsrc->numnodes=2;
	Node_Entry *N1=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N2=(struct node_s*) malloc(sizeof(struct node_s*) );
	N1->name=node1;
	N2->name=node2;
	N1->next=N2;
	N2->next=NULL;
	nodelist=N1;
        deviceIsrc->nodelist = nodelist;
	deviceIsrc->value=value;
	deviceIsrc->next=NULL;

	Isrctail->next=deviceIsrc;
	Isrctail=deviceIsrc;

	Nodetail->next=N1;
	Nodetail=N2;
}

void ParseVCCS(char *name, char *node1, char *node2, char *node3, char *node4, double value)
{
	int numnodes;
	Node_Entry *nodelist;
	Device_Entry *deviceVCCS;
	
	printf("[VCCS parsed ...]\n");
	printf("   name=%s, N+=%s, Ne-=%s,  Nc+=%s, Nc-=%s, G=%e\n", 
			name, node1, node2, node3, node4, value);
	nVCCS++;
	deviceVCCS = (struct device_s *) malloc(sizeof(struct device_s));
	deviceVCCS->name=name;
	deviceVCCS->numnodes=4;
	Node_Entry *N1=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N2=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N3=(struct node_s*) malloc(sizeof(struct node_s*) );
	Node_Entry *N4=(struct node_s*) malloc(sizeof(struct node_s*) );
	N1->name=node1;
	N2->name=node2;
	N3->name=node3;
	N4->name=node4;
	N1->next=N2;
	N2->next=N3;
	N3->next=N4;
	N4->next=NULL;
	nodelist=N1;
        deviceVCCS->nodelist = nodelist;
	deviceVCCS->value=value;
	deviceVCCS->next=NULL;

	VCCStail->next=deviceVCCS;
	VCCStail=deviceVCCS;

	Nodetail->next=N1;
	Nodetail=N4;
}


void Summarize()
{
	printf("[Finished parsing netlist!]\n");
	printf("   #res=%d, #cap=%d, #ind=%d, #vccs=%d, #vsrc=%d, #isrc=%d\n", \
		nRes, nCap, nInd, nVCCS, nVsrc, nIsrc);
}
