#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

TreeError createTree(Tree * newTree, const char * pathToLog, const char * file, int line)
{
    if (!newTree) return NULL_PTR;

    newTree->logFile           = fopen(pathToLog, "w");
    newTree->numberOfOperation = 0;

    if (!newTree->logFile) {
        newTree->codeOfError = LOG_FAIL;
        return LOG_FAIL;
    }

    fprintf(newTree->logFile, "<pre><span style=\"font-size: 16px\">\n\n");

    DO_LOG_DUMP(line, file, newTree)
    
    return NORMAL;
}

TreeError createNode(Tree * tree, treeElem_t newElement, Node * parentElement, bool answer, const char * file, const int line)
{
    if ( !tree ) return NULL_PTR;
    
    Node * newNode = tree->firstNode;
    if ( tree->firstNode ) {
        newNode = (Node *)calloc(1, sizeof(Node));
        if (!newNode) return NULL_CALLOC;
    }

    newNode->data  = strdup(newElement);
    if (!newNode->data) return NULL_CALLOC;

    if ( tree->firstNode ) {
        if (answer == 0) parentElement->right = newNode;
        else             parentElement->left  = newNode;
    }

    DO_LOG_DUMP(line, file, tree);

    return NORMAL;
}

TreeError destructTree(Tree * tree)
{
    _freeNode(tree->firstNode);
    fclose(tree->logFile);

    return NORMAL;
}

void _freeNode(Node * node)
{
    if (node->left) _freeNode(node->left);
    if (node->right) _freeNode(node->right);

    free(node);
}