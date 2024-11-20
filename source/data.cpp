#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "tree.h"
#include "data.h"

static void _printNode(FILE * outputFile, Node * nodeForPrint, size_t depthOfTree);

Node * _readFromFile(FILE * inputFile, char * prevVector, char answer)
{
    int c = 0;
    while ( (c = fgetc(inputFile)) != '{');
    Node * node = (Node *)calloc(1, sizeof(Node));
    _readDataToNode(inputFile, node);

    int skipInput = 1;
    while (skipInput) {
        c = fgetc(inputFile);
        if (c == '{' || c == '}') skipInput = 0; 
    }

    if (c == '}') {
        node->pathVector = getVector(prevVector, answer);
        return node;
    } 

    ungetc(c, inputFile);

    node->pathVector = getVector(prevVector, answer);
    node->left  = _readFromFile(inputFile, node->pathVector, 1);
    node->right = _readFromFile(inputFile, node->pathVector, 0);

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

    tree->firstNode = _readFromFile(inputFile, NULL, 0);

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
    printf("I won! I know all!\n");
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

    node->left->pathVector  = getVector(node->pathVector, 1);
    node->right->pathVector = getVector(node->pathVector, 0);

    printf("Okay, now I know more than before, thank you!\n");
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

void startGame() {
    bool run = 1;
    int c = 0;

    Tree * tree = readFromFile(fopen("data.txt", "r"), "log/logfile.htm");

    if ( !tree ) return printUnknownError();

    while (run) {
        printMainMenu();
        c = fgetc(stdin);

        switch (c) {
            case 'g': {
                runGuessMode(tree);
                skipInput();
                break;
            }
            case 'd': {
                runDefMode(tree);
                skipInput();
                break;
            }
            case 'c': {
                runCompareMode(tree);
                skipInput();
                break;
            }
            case 'l': {
                showLog(tree);
                skipInput();
                break;
            }
            case 's': {
                FILE * dataFile = fopen("data.txt", "w");
                printNode(dataFile, tree->firstNode);

                run = 0;

                break;
            }
            case 'e': {
                run = 0;

                break;
            }
            default: {
                printInputError();
                break;
            }
        }
    }
}

void printMainMenu() 
{
    printf("Hello to Akinator! I can guess your character! What do you want?\n");
    printf("\t[g] - guess mode\n");
    printf("\t[d] - definition mode\n");
    printf("\t[c] - compare mode\n");
    printf("\t[l] - show picture of tree\n");
    printf("\t[s] - save and exit\n");
    printf("\t[e] - exit without saving\n");
}

void printInputError()
{
    printf("I can`t understand you! Choose wisely!");
}

void printUnknownError()
{
    printf("Akinator is not available now! Please, make an issue on my github\n");
}

void runGuessMode(Tree * tree)
{
    Node * lastNode = askQuestions(tree->firstNode);

    int answer = 0;
    scanf("%d", &answer);

    if (answer == 1) winScript();
    else             defeatScript(tree, lastNode);
}

void runDefMode(Tree * tree)
{
    skipInput();
    printf("Enter the name of subject, about which you want to know more!\n");
    char buf[sizeOfBuf] = {};
    fgets(buf, sizeOfBuf, stdin);

    size_t lenghtOfSubject = strlen(buf);
    buf[lenghtOfSubject - 1] = '\0';

    char * path = findPath(buf, tree->firstNode);

    if (path == NULL) {
        printf("This subject does not exist! Or I just don`t know about it...\n");
        return;
    }

    Node * actualNode = tree->firstNode;

    printf("%s [%s]:", buf, path);

    for (int i = 0; i < sizeOfBuf - 1; i++) {
        if (path[i] == '\0') {
            printf(".\n"); return;
        }
        if (path[i] == 'y') {
            printf(" %s", actualNode->data);
            actualNode = actualNode->left;
        }
        else if (path[i] == 'n') {
            printf(" NOT %s", actualNode->data);
            actualNode = actualNode->right;
        }
    }


}

void runCompareMode(Tree * tree)
{
    skipInput();

    printf("Write the name of first subject:\n");
    char firstBuf[sizeOfBuf] = {};
    fgets(firstBuf, sizeOfBuf, stdin);

    size_t firstLenghtOfSubject = strlen(firstBuf);
    firstBuf[firstLenghtOfSubject - 1] = '\0';

    printf("Write the name of second subject:\n");
    char secondBuf[sizeOfBuf] = {};
    fgets(secondBuf, sizeOfBuf, stdin);

    size_t secondLenghtOfSubject = strlen(secondBuf);
    secondBuf[secondLenghtOfSubject - 1] = '\0';

    char * firstPath = findPath(firstBuf, tree->firstNode);
    char * secondPath = findPath(secondBuf, tree->firstNode);

    if (!secondPath && !firstPath) {
        printf("I don`t know both of subjects!\n");
        return;
    }
    if (!firstPath) {
        printf("I don`t know first subject!\n");
        return;
    }
    if (!secondPath) {
        printf("I don`t know second subject!\n");
        return;
    }

    if (firstPath == secondPath) {
        printf("It is one object!\n");
        return;
    }

    if (secondPath[0] != firstPath[0]) {
        printf("Objects are not similar to each other!\n");
    }

    printf("%s is similar to %s, because they are both", firstBuf, secondBuf);

    Node * actualNode = tree->firstNode;

    size_t minLenght = minOfTwoLenght(firstPath, secondPath);

    for (size_t i = 0; i < minLenght - 1; i++) {
        printf(" %s,", actualNode->data);
        actualNode = (firstPath[i] == 'y') ? actualNode->left : actualNode->right;
    }

    if (firstPath[minLenght] == 'y') printf(" but %s is %s", firstBuf,  actualNode->data);
    else                             printf(" but %s is %s", secondBuf, actualNode->data);

    return;
}

/*size_t compareNodes(char * firstVector, char * secondVector)
{
    size_t minLenght = minOfTwoLenght(firstVector, secondVector);

    for (size_t i = 0; i < minLenght + 1; i++)
        if (firstVector[i] != secondVector[i]) return i;

    return minLenght;
}*/

size_t minOfTwoLenght(char * firstVector, char * secondVector)
{
    size_t first  = strlen(firstVector);
    size_t second = strlen(secondVector);

    return first*(first < second) + second * (first >= second);
}

void skipInput()
{
    int c = 0;
    while ( (c = fgetc(stdin)) != '\n');
}

void showLog(Tree * tree)
{
    char dotFile[sizeOfBuf] = {};
    char pngFile[sizeOfBuf] = {};

    snprintf(dotFile, sizeOfBuf, "log/req.dot");
    snprintf(pngFile, sizeOfBuf, "log/req.png");

    dump(tree, dotFile);

    char cmd[sizeOfBuf] = "dot -Tpng -o ";
    strcat(cmd, pngFile);
    strcat(cmd, " ");
    strcat(cmd, dotFile);

    system(cmd);

    system("xdg-open log/req.png");
}

char * getVector(char * prevVector, char answer)
{
    if (prevVector)
    {
        size_t lenght = strlen(prevVector);

        char * newVector = (char *)calloc(lenght + 1, sizeof(char));

        strcpy(newVector, prevVector);
        if (answer) newVector[lenght] = 'y';
        else        newVector[lenght] = 'n';

        return newVector;
    }

    char * newVector = (char *)calloc(1, sizeof(char));
    newVector[0] = '\0';

    return newVector;
}

char * findPath(const char * nameOfNode, Node * actualNode)
{
    if (strcmp(nameOfNode, actualNode->data) == 0 && actualNode->left == NULL && actualNode->right == NULL) {
        return actualNode->pathVector;              
    }

    if (strstr(actualNode->pathVector, "y") == NULL && actualNode->left == NULL && actualNode->right == NULL) {
        return NULL;
    }

    char * resultLeft = (actualNode->left) ? findPath(nameOfNode, actualNode->left): NULL;
    char * resultRight = (actualNode->right) ? findPath(nameOfNode, actualNode->right):NULL;

    if (resultLeft) return resultLeft;
    return resultRight;
}