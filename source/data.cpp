#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "tree.h"
#include "data.h"

static void _printNode(FILE * outputFile, Node * nodeForPrint, size_t depthOfTree);

Node * _readFromFile(FILE * inputFile)
{
    int c = 0;
    //printf("huy\n");
    while ( (c = fgetc(inputFile)) != '{');
    Node * node = (Node *)calloc(1, sizeof(Node));
    _readDataToNode(inputFile, node);

    int skipInput = 1;
    while (skipInput) {
        c = fgetc(inputFile);
        if (c == '{' || c == '}') skipInput = 0;
    }

    if (c == '}') return node;

    ungetc(c, inputFile);
    node->left  = _readFromFile(inputFile);
    node->right = _readFromFile(inputFile);

    return node;
}

void _readDataToNode(FILE * inputFile, Node * node)
{
    int c = 0;

    while ( (c = fgetc(inputFile)) != '"');
    node->data = (char *)calloc(sizeOfBuf, sizeof(char));

    printf("%p\n", node->data);
    fscanf(inputFile, "%[^\"]", node->data);
}

Tree * readFromFile(FILE * inputFile, const char * pathToLog)
{
    Tree * tree = (Tree *)calloc(1, sizeof(Tree));

    DO_CREATE_TREE(tree, pathToLog);

    tree->firstNode = _readFromFile(inputFile);

    fclose(inputFile);

    return tree;
}

// [s] - save and exit, [e] - exit, [c] - compare, [l] - dump, []

Node * askQuestions(Node * actualNode)
{
    if (actualNode == NULL) return NULL;

    if (actualNode->left == 0 && actualNode->right == 0) {
        printf("Is it %s?\n", actualNode->data);
        return actualNode;
    }

    printf("%s?\n", actualNode->data);

    int result = 0;
    scanf("%d", &result);

    if (result == 1) return askQuestions(actualNode->left);
    else             return askQuestions(actualNode->right);
}

void winScript()
{
    printf("I won! Do you want to continue the game?\n");
}

void defeatScript(Tree * tree, Node * node)
{
    printf("Oh no! Who did you guessed?\n");

    char buf[sizeOfBuf] = {};
    fgets(buf, sizeof(buf), stdin);
    fgets(buf, sizeof(buf), stdin);

    size_t lenghtOfBuf = strlen(buf);
    char * rightAnswer = strndup(buf, lenghtOfBuf-1);

    char * wrongAnswer = node->data;

    DO_CREATE_NODE(tree, wrongAnswer, node, 0);
    DO_CREATE_NODE(tree, rightAnswer, node, 1);

    printf("And what is difference between %s and %s?\n", rightAnswer, wrongAnswer);

    char * dif = readDifference();
    node->data = dif;

    printf("Okay, now I know more than before, thank you! Do you want to continue the game?");
}

char * readDifference() 
{
    char buf[sizeOfBuf] = {};
    fgets(buf, sizeof(buf), stdin);
    size_t lenghtOfBuf = strlen(buf);

    return strndup(buf, lenghtOfBuf-1);
}


int writeUsageError()
{
    fprintf(stderr, "USAGE: akinator <flag>\n");
    fprintf(stderr, "Accessible flags:\n");
    fprintf(stderr, "\t -g - guess mode");
    fprintf(stderr, "\t -d - definition mode");

    return -1;
}

static void _printNode(FILE * outputFile, Node * nodeForPrint, size_t depthOfTree)
{
    if (nodeForPrint == NULL) return;

    printTab(outputFile, depthOfTree);
    fprintf(outputFile, "{\n");

    printTab(outputFile, depthOfTree + 1);
    fprintf(outputFile, "\"%s\"\n", nodeForPrint->data);

    _printNode(outputFile, nodeForPrint->left,  depthOfTree + 1);
    _printNode(outputFile, nodeForPrint->right, depthOfTree + 1);

    printTab(outputFile, depthOfTree);
    fprintf(outputFile, "}\n");
}

void printTab(FILE * outputFile, size_t depthOfTree)
{
    for (size_t i = 0; i < depthOfTree; i++) fprintf(outputFile, "\t");
}

void printNode(FILE * outputFile, Node * firstNode)
{
    _printNode(outputFile, firstNode, 0);
}