#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include "tree.h"

Node * askQuestions(Node * actualNode);

void _readDataToNode(FILE * inputFile, Node * node);

Tree * readFromFile(FILE * inputFile, const char * pathToLog);

Node * _readFromFile(FILE * inputFile);

void winScript();

void defeatScript(Tree * tree, Node * node);

char * readDifference();

int writeUsageError();

void printTab(FILE * outputFile, size_t depthOfTree);

void printNode(FILE * outputFile, Node * firstNode);

#endif // DATA_H
