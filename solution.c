/*
*
* @author Matúš Vráblik <xvrabl05@vutbr.cz>
*/
#include <string.h>
#include "solution.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ctype.h>
int** parseGraph(Array *grafy){
    int j = 0;
    int** matrixgraph = (int **) malloc(grafy->pocetriadkov+1 * sizeof(int *));
    for (int i = 0; i < grafy->pocetriadkov; ++i){   
        int x = 0;
        int str_length = strlen(grafy->data[i]);
        //    fprintf(stdout,"----%d.---- >/", str_length);
        int *riadok = (int *) malloc (str_length * sizeof (int));
        matrixgraph[i] = (int *) malloc(1+str_length * sizeof(int));
        int o = 2;
        //int matrixgraph[i][str_length];

        for (j = 0; grafy->data[i][j] != '\0'; j++) {
                
            if (isdigit(grafy->data[i][j])){
                //fprintf(stdout,"na3iel som cislo>%d", grafy->data[i][j]);
                *riadok = grafy->data[i][j];

                x =  (x*10 + *riadok - '0' );
                //fprintf(stdout,"rozsirujem cislo>%d \n", x); 
                
                matrixgraph[i][o-1] = x ;
                //fprintf(stdout,"vkladam cislo>%d na %dx%d\n", x,i,o-1);                     
            }
            else { 

                x = 0;
               
                o++;
                 matrixgraph[i][0] = o - 3;
            }

        }
        free(riadok);
    }
    int a,b;
    //printf("\n");
    for(a=0;a<grafy->pocetriadkov;a++)
     {
          
          for(b=0;b<= matrixgraph[a][0]+1;b++)
          {
               //printf("%3d ",matrixgraph[a][b]);
               /*Here, %3d takes 3 digit space for each digit while printing  output */
          }
          //printf("\n");

    }
    return matrixgraph;
    
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
void rmEdge(point* P1,point* P2){    
    int actDegree=P1->degree;
    int j=0;
    while(j<actDegree){
        if(P1->edges[j]->num==P2->num){
            P1->edges[j]=NULL;
            P1->degree--;
        }
        j++;
    }             
    rearrEdges(P1,actDegree); 
}
void tieEdge(point* P1,point* P2){
    bool haveEdge=false;
    int i=0;
    while(!haveEdge&&i<P1->degree){
        if(P1->edges[i]==P2){
            haveEdge=true;
        }
        i++;
    }
    if(!haveEdge){
        addEdge(P1,P2);
        addEdge(P2,P1);
    }
}
void rmPoint(graph* G,int i){
    point** newPoints=(point**)malloc(sizeof(point)*(G->p_sum-1));
    if(G->points[i]->degree>0){
        rmEdge(G->points[i]->edges[0],G->points[i]);
    }
    if(G->points[i]->degree>1){
        rmEdge(G->points[i]->edges[1],G->points[i]);
        tieEdge(G->points[i]->edges[0],G->points[i]->edges[1]);
    }
    point** tmp=G->points;
    for(int j=0;j<i;j++){
        newPoints[j]=G->points[j];
    }
    for(int j=i+1;j<G->p_sum;j++){
        newPoints[j-1]=G->points[j];
    }
    free(G->points[i]->edges);
    free(G->points[i]);
    G->points=newPoints;
    G->p_sum--;
    free(tmp);
}
void rmExcessPoints(graph* G){
    int i=0;
    while(i<G->p_sum&&G->p_sum>2){
        if(G->points[i]->degree<3){
            rmPoint(G,i);
            i--;
        }
        i++;
    }
}
void rmExcessEdges(graph* G){
    for(int i=0;i<G->p_sum;i++){
        rmEdge(G->points[i],G->points[i]);
    }
    int *edges=malloc(sizeof(int)*G->p_sum);
    for(int j=0;j<G->p_sum;j++){
        for(int i=0;i<G->p_sum;i++){
            edges[i]=0;
        }
        for(int k=0;G->points[j]->edges[k];k++){
            edges[G->points[j]->edges[k]->num-1]++;
        }
        for(int k=0;k<G->p_sum;k++){
            if(edges[k]>1){
                rmEdge(G->points[j],G->points[k]);
                addEdge(G->points[j],G->points[k]);
            }
        }
    }
    free(edges);
}
void freeGraph(graph* G){
    for(int i=0;i<G->p_sum;i++){
        free(G->points[i]->edges);
        free(G->points[i]);
    }
    free(G->points);
    free(G);
}
void freeParsed(int** matrix){
    int i=0;
    while(matrix[i]){
        free(matrix[i]);
        i++;
    }
    free(matrix);
}
void initGraph(graph *G,int** matrixGraph,int rowCount){
    G->p_sum=rowCount;
    G->points=(point **)malloc(sizeof(point)*G->p_sum);
    for(int i=0;i<G->p_sum;i++){
        G->points[i]=(point *)malloc(sizeof(point));
        G->points[i]->degree=matrixGraph[i][0];
        G->points[i]->num=matrixGraph[i][1];
        G->points[i]->edges=(point **)malloc(sizeof(point)*G->points[i]->degree);
        for(int j=0;j<G->points[i]->degree;j++){
            G->points[i]->edges[j]=G->points[matrixGraph[i][j+2]-1];
        }
        if(i+1!=G->p_sum){
            G->points[i]->n_point=G->points[i+1];
        }
        else{
            G->points[i]->n_point=NULL;
        }
    };
    for(int i=0;i<G->p_sum;i++){
        for(int j=0;j<G->points[i]->degree;j++){
            G->points[i]->edges[j]=G->points[matrixGraph[i][j+2]-1];
        }
        if(i+1!=G->p_sum){
            G->points[i]->n_point=G->points[i+1];
        }
        else{
            G->points[i]->n_point=NULL;
        }
    };
    G->first_p=G->points[0];
    freeParsed(matrixGraph);
}
void solvegraph(Array *grafy,int *beginGraphs){
    int s=1;
    graph *Graph=(graph *)malloc(sizeof(graph));
    initGraph(Graph,parseGraph(grafy),grafy->pocetriadkov);//workaround nonexistent input
    fprintf(stdout,"Graph p_sum %d\n",Graph->p_sum);
    if(Graph->p_sum>2){//true change for "Graph->p_sum>5"
        rmExcessEdges(Graph);
        checkEdgePoints(Graph);
    }
    for(int i=0;i<Graph->p_sum;i++){
        printf("Point:%ld|",Graph->points[i]->num);
        for(int j=0;j<Graph->points[i]->degree;j++){
            if(Graph->points[i]->edges[j]!=NULL){
                printf("%ld ",Graph->points[i]->edges[j]->num);                
            }
        }
        printf("\n");
    }
    printf("\n");
    
    rmExcessPoints(Graph);
    /*while(Graph->p_sum>2){
        s=0;
    }*/

    //workaround check
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