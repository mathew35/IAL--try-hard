/*
*
* @author Matúš Vráblik <xvrabl05@vutbr.cz>
*/
#include "array.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
typedef struct tpoint{
    size_t num;
    struct tpoint** edges;
    size_t edgesmemory;
    size_t degree;
}point;
typedef struct{
    int p_sum;
    point** points;
}graph;

void solvegraph(Array *grafy,int *arr,int);