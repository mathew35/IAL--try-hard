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
    struct tpoint* n_point;
    size_t degree;
    bool visited;
}point;
typedef struct{
    int p_sum;
    point** points;
    point*  first_p;
}graph;

void solvegraph(Array *grafy,int *arr,int);