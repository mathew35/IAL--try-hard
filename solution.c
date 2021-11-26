/*
*
* @author Matúš Vráblik <xvrabl05@vutbr.cz>
*/

#include "solution.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void initGraph(graph *G){
    G->p_sum=4;
    G->points=(point **)malloc(sizeof(point)*4);
    for(int i=0;i<G->p_sum;i++){
        G->points[i]=(point *)malloc(sizeof(point));
    };
    G->first_p=G->points[0];
    G->points[0]->degree=2;
    G->points[0]->num=1;

    G->points[1]->degree=3;
    G->points[1]->num=2;

    G->points[2]->degree=1;
    G->points[2]->num=3;

    G->points[3]->degree=3;
    G->points[3]->num=4;

    G->points[0]->edges=(point **)malloc(sizeof(point)*2);
    G->points[0]->edges[0]=G->points[1];
    G->points[0]->edges[1]=G->points[3];
    G->points[0]->n_point=G->points[1];


    G->points[1]->edges=(point **)malloc(sizeof(point)*3);
    G->points[1]->edges[0]=G->points[3];
    G->points[1]->edges[1]=G->points[2];
    G->points[1]->edges[2]=G->points[0];
    G->points[1]->n_point=G->points[2];

    G->points[2]->edges=(point **)malloc(sizeof(point)*1);
    G->points[2]->edges[0]=G->points[1];
    G->points[2]->n_point=G->points[3];

    G->points[3]->edges=(point **)malloc(sizeof(point)*3);
    G->points[3]->edges[0]=G->points[3];
    G->points[3]->edges[1]=G->points[2];
    G->points[3]->edges[2]=G->points[3];
    G->points[3]->n_point=NULL;
}
void rearrEdges(point* P,int oldDegree){
    point** newEdges=(point **)malloc(sizeof(point)*P->degree);
    if(newEdges!=NULL){
        int nEcounter=0;
        for(int i=0;i<oldDegree;i++){
            if(P->edges[i]!=NULL){
                newEdges[nEcounter]=P->edges[i];
                nEcounter++;
            }            
        }
        free(P->edges);
        P->edges=newEdges;
    }
}
void addEdge(point* P1,point* P2){
    printf("Adding EdgePoint %ld to point %ld\n",P2->num,P1->num);
    point** newEdges=malloc(sizeof(point)*(P1->degree+1));
    for(int i=0;i<P1->degree;i++){
        newEdges[i]=P1->edges[i];
    };
    newEdges[P1->degree]=P2;
    point** tmp=P1->edges;
    P1->edges=newEdges;
    P1->degree++;
    free(tmp);
}
void checkEdgePoints(graph *G){
    for(int i=0;i<G->p_sum;i++){
        for(int j=0;j<G->points[i]->degree;j++){
            bool found=false;
            int k=0;
            while((k++<G->points[i]->edges[j]->degree)&&(!found)){
                if(G->points[i]->edges[j]->edges[k-1]==G->points[i]){
                    found=true;
                }
            }
            if(!found){ 
                addEdge(G->points[i]->edges[j],G->points[i]);
            }
        }
    }
}
void freeGraph(graph* G){
    for(int i=0;i<G->p_sum;i++){
        free(G->points[i]->edges);
        free(G->points[i]);
    }
    free(G->points);
    free(G);
}
void solvegraph(){
    int s=1;
    graph *Graph=(graph *)malloc(sizeof(graph));
    initGraph(Graph);//workaround nonexistent input
    fprintf(stdout,"Graph p_sum %d\n",Graph->p_sum);
    while(Graph->p_sum>1){
        for(int i=0;i<Graph->p_sum;i++){
            bool rm=false;
            int actDegree=Graph->points[i]->degree;
            for(int j=0;j<actDegree;j++){
                if(Graph->points[i]->edges[j]->num==Graph->points[i]->num){
                    //#1 problem: removing connections SOLVED!
                    Graph->points[i]->edges[j]=NULL;
                    Graph->points[i]->degree--;
                    rm=true;
                }
            }
            if(rm){
                rearrEdges(Graph->points[i],actDegree);            
            }
            if(Graph->points[i]->degree<3){
                if(Graph->points[i]->degree==1){
                }
                if(Graph->points[i]->degree==2){                    
                }
            }
        }
        checkEdgePoints(Graph);
        s=0;
        Graph->p_sum=0;
    }

    //workaround check
    Graph->p_sum=4;
    for(int i=0;i<Graph->p_sum;i++){
        printf("Point:%ld|",Graph->points[i]->num);
        for(int j=0;j<Graph->points[i]->degree;j++){
            if(Graph->points[i]->edges[j]!=NULL){
                printf("%ld ",Graph->points[i]->edges[j]->num);                
            }
        }
        printf("\n");
    }
    freeGraph(Graph);
    char *answer[2]={"non-planar","planar"};
    fprintf(stdout,"Graph is %s\n",answer[s]);
}