#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#define DO_CREATE_TREE(tree, pathToLog) {                           \
	createTree(tree, pathToLog, __FILE__, __LINE__);                \
}

#define DO_CREATE_NODE(tree, newElement, parent, answer) {                               \
	createNode(tree, newElement, parent, answer, __FILE__, __LINE__);                    \
}

typedef char * treeElem_t;

typedef struct Node
{
    treeElem_t data;
    struct Node * left;
    struct Node * right;
} Node;

enum TreeError 
{
    NORMAL      = 0,
    NULL_CALLOC = 1 << 1,
    LOG_FAIL    = 1 << 2,
    NULL_PTR    = 1 << 3,
    UNKNOWN_ERR = 1 << 4
};

typedef struct Tree
{
    Node * firstNode;
    TreeError codeOfError;

    FILE * logFile;
    int numberOfOperation;
} Tree;

TreeError createTree(Tree * newTree, const char * pathToLog, const char * file, int line);

TreeError createNode(Tree * tree, const treeElem_t newElement, Node * parentElement, bool answer, const char * file, const int line);

TreeError destructTree(Tree * tree);

void _freeNode(Node * node);

#endif // TREE_H
