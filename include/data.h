#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include "tree.h"

Node * askQuestions(Node * actualNode);

void _readDataToNode(FILE * inputFile, Node * node);

Tree * readFromFile(FILE * inputFile, const char * pathToLog);

Node * _readFromFile(FILE * inputFile, char * prevVector, char answer);

void winScript();

void defeatScript(Tree * tree, Node * node);

char * readDifference();

int writeUsageError();

void printTab(FILE * outputFile, size_t depthOfTree);

void printNode(FILE * outputFile, Node * firstNode);

void printMainMenu();

void runGuessMode(Tree * tree);

void printUnknownError();

void printInputError();

void runDefMode(Tree * tree);

void runCompareMode(Tree * tree);

size_t compareNodes(char * firstVector, char * secondVector);

size_t minOfTwoLenght(char * firstVector, char * secondVector);

void startGame();

void skipInput();

void showLog(Tree * tree);

char * getVector(char * prevVector, char answer);

char * findPath(const char * nameOfNode, Node * actualNode);

#endif // DATA_H
