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
            rearrEdges(P1,actDegree);
            actDegree--;
        }
        j++;
    }          
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
void freeGraph(graph* G){
    for(int i=0;i<G->p_sum;i++){
        free(G->points[i]->edges);
        free(G->points[i]);
    }
    free(G->points);
    free(G);
}

void solvegraph(Array *grafy){

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
                fprintf(stdout,"na3iel som cislo>%d", grafy->data[i][j]);
                *riadok = grafy->data[i][j];

                x =  (x*10 + *riadok - '0' );
                fprintf(stdout,"rozsirujem cislo>%d \n", x); 
                
                matrixgraph[i][o-1] = x ;
                fprintf(stdout,"vkladam cislo>%d na %dx%d\n", x,i,o-1);                     
            }
            else { 

                x = 0;
               
                o++;
                 matrixgraph[i][0] = o - 3;
            }

        }
    }
    int a,b;
    printf("\n");
    for(a=0;a<grafy->pocetriadkov;a++)
     {
          
          for(b=0;b<= matrixgraph[a][0];b++)
          {
               printf("%3d ",matrixgraph[a][b]);
               /*Here, %3d takes 3 digit space for each digit while printing  output */
          }
          printf("\n");

    }
    int s=1;
    graph *Graph=(graph *)malloc(sizeof(graph));
    initGraph(Graph);//workaround nonexistent input
    fprintf(stdout,"Graph p_sum %d\n",Graph->p_sum);
    if(true){//true change for "Graph->p_sum>5"
        for(int i=0;i<Graph->p_sum;i++){
<<<<<<< HEAD
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
=======
            rmEdge(Graph->points[i],Graph->points[i]);
>>>>>>> b599bd136a2e6f83dd0ec87a7d2ffee39114bd43
        }
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