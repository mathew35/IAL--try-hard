/*
 * David Novak
 * xnovak2r
 *
 * Random Graph File Generator
 * IAL alternative project
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_VALUE 100

unsigned RNG(unsigned defaultValue){
    unsigned randNum = (rand() % defaultValue + 1);
    return randNum;
}

int main(int argc, char* argv[]){
    /* Argument handling */
    switch (argc) {
        case 4:
            break;
        case 2:
            if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0){
                printf("\nRun with 3 arguments\n");
                printf("Argument 1 - number of graphs ∈ <1;5000>\n");
                printf("Argument 2 - max number of nodes in graph ∈ <1;2000>\n");
                printf("Argument 3 - max degree of graph node ∈ <1;1000>\n");
                printf("If any argument is NaN, the sequence of numbers that the NaN expression begins with will be used\n");
                printf("If any argument is out of range of allowed values, default value will be used\n\n");
                goto end;
            }
        default:
            printf("Try -h as argument\n");
            goto end;
    }

    FILE *fp;
    unsigned graphCount, maxNumberOfNodes, maxNodeDegree, randomNodeNumber, nodeDegree, randomNode;
    //char *hashtag = "###########################################\n";

    /* Check of values given in argument */
    graphCount = strtoul(argv[1], NULL, 10);
    if(graphCount > 5000 || graphCount <= 0) graphCount = DEFAULT_VALUE;

    maxNumberOfNodes = strtoul(argv[2], NULL, 10);
    if(maxNumberOfNodes > 2000 || maxNumberOfNodes <= 0) maxNumberOfNodes = DEFAULT_VALUE;

    maxNodeDegree = strtoul(argv[3], NULL, 10);
    if(maxNodeDegree > 1000 || maxNodeDegree <= 0) maxNodeDegree = DEFAULT_VALUE;

    //printf("graphCount: %u\nmaxNumberOfNodes: %u\nmaxNodeDegree: %u\n", graphCount, maxNumberOfNodes, maxNodeDegree);

    /* File opening and ERROR check */
    fp = fopen("graph.txt", "w");
    if(fp == NULL){
        printf("ERROR: File creation\n");
        return 1;
    }
    
    /* set seed to get random numbers */
    srand(time(NULL));
    
    /* Now the generating of graphs begins */
    for (unsigned i = 1; i <= graphCount; ++i) {
        /* Print of:
         * ############
         * # Graph %i
         */
        //fputs(hashtag, fp);

        char graphNum[5], graph[20] = "# Graph ";
        sprintf(graphNum, "%d\n", i);
        strcat(graph, graphNum);
        fputs(graph, fp);

        /* Generate random number of nodes in graph ∈ <1; maxNumberOfNodes> */
        randomNodeNumber = RNG(maxNumberOfNodes);

        /* For each node in graph */
        for (unsigned j = 1; j <= randomNodeNumber; ++j) {
            /* Print: %j| */
            fprintf(fp, "%d|", j);
            fflush(fp);

            /* And generate random number of connections */
            nodeDegree = RNG(maxNodeDegree);
            for (unsigned k = 1; k < nodeDegree; ++k) {
                randomNode = RNG(randomNodeNumber);
                fprintf(fp, "%u,", randomNode);
                fflush(fp);
            }
            randomNode = RNG(randomNodeNumber);
            fprintf(fp, "%u\n", randomNode);
            fflush(fp);
        }
    }

    fclose(fp);
end:
    return 0;
}
