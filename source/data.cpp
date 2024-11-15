#include <stdio.h>
#include <string.h>
#include "log.h"
#include "tree.h"
#include "data.h"

static void _printNode(FILE * outputFile, Node * nodeForPrint, size_t depthOfTree);

//Tree * readFromData(FILE * inputFile, )
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
    const char * rightAnswer = strndup(buf, lenghtOfBuf-1);

    const char * wrongAnswer = node->data;

    DO_CREATE_NODE(tree, wrongAnswer, node, 0);
    DO_CREATE_NODE(tree, rightAnswer, node, 1);

    printf("And what is difference between %s and %s?\n", rightAnswer, wrongAnswer);

    const char * dif = readDifference();
    node->data = dif;

    printf("Okay, now I know more than before, thank you! Do you want to continue the game?");
}

const char * readDifference() 
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