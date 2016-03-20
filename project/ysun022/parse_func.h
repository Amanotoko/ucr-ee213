/************************************************************
  	parse_func.h
	header for parse_func.c

Author(s):
   	Shi, Guoyong, 2015
 
	Supporting course material for "Introduction to EDA", 2015
   	Dept of Micro/Nano-electronics
   	Shanghai Jiao Tong University (SJTU)
************************************************************/

#ifndef PARSE_FUNC_H
#define PARSE_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include "Symbol_Table.h"

// Global variables defined for the parser utility functions
extern int nRes;
extern int nCap;
extern int nInd;
extern int nVsrc;
extern int nIsrc;
extern int nVCCS;
extern Node_Entry **NodeTable;
Device_Entry *headRes, *tailRes;
Device_Entry *headCap, *tailCap;
Device_Entry *headCapIC, *tailCapIC;
Device_Entry *headInd, *tailInd;
Device_Entry *headIndIC, *tailIndIC;
Device_Entry *headVsrc, *tailVsrc;
Device_Entry *headIsrc, *tailIsrc;
Device_Entry *headVCCS, *tailVCCS;
Node_Entry *headNode, *tailNode;

extern void Init_parse_util();
extern void ParseRes(char *str, char *node1, char *node2, double value);
extern void ParseCap(char *str, char *node1, char *node2, double value);
extern void ParseCapIC(char *str, char *node1, char *node2, double value, double init);
extern void ParseInd(char *str, char *node1, char *node2, double value);
extern void ParseIndIC(char *str, char *node1, char *node2, double value, double init);
extern void ParseVsrc(char *str, char *node1, char *node2, double value);
extern void ParseIsrc(char *str, char *node1, char *node2, double value);
extern void ParseVCCS(char *str, char *node1, char *node2, char *node3, char *node4, double value);
extern void Summarize();

#endif
