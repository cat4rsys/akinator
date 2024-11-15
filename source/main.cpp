#include "tree.h"
#include "data.h"
#include <stdio.h>

int main(const int argc, const char * argv[]) 
{
    Tree * tree = readFromFile(fopen("data.txt", "r"), "log/logfile.htm");

    int run = 1;

    while (run) {
        Node * lastNode = askQuestions(tree->firstNode);

        int answer = 0;
        scanf("%d", &answer);

        if (answer == 1) winScript();
        else             defeatScript(tree, lastNode);
        scanf("%d", &run);
    }

    FILE * dataFile = fopen("data.txt", "w");
    printNode(dataFile, tree->firstNode);

    //if (argc != 2) return writeUsageError();
    // readData();


}