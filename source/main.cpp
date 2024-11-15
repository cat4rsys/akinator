#include "tree.h"
#include "data.h"
#include <stdio.h>

int main(const int argc, const char * argv[]) 
{
    Tree tree = {};

    DO_CREATE_TREE(&tree, "log/logfile.htm", "Animal");
    DO_CREATE_NODE(&tree, "Human", tree.firstNode, 0);
    DO_CREATE_NODE(&tree, "Poltorashka", tree.firstNode, 1);

    int run = 1;

    while (run) {
        Node * lastNode = askQuestions(tree.firstNode);

        int answer = 0;
        scanf("%d", &answer);

        if (answer == 1) winScript();
        else             defeatScript(&tree, lastNode);
        scanf("%d", &run);
    }

    FILE * dataFile = fopen("data.txt", "w");
    printNode(dataFile, tree.firstNode);

    //if (argc != 2) return writeUsageError();
    // readData();


}