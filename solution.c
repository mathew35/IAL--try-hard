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
int** parseGraph(Array *grafy,int * beginG,int Gcount, int index,int rowcount){
    int j = 0;
    int** matrixgraph = (int **) malloc(rowcount * sizeof(int *));
    for(int i=0;i<rowcount;i++){
        matrixgraph[i]=NULL;
    }
    for (int i = beginG[index]+1; i < beginG[index]+rowcount; ++i){   
        int x = 0;
        int ii=i-beginG[index];
        int str_length = strlen(grafy->data[i]);
        int *line = (int *) malloc (str_length * sizeof (int));
        matrixgraph[ii-1] = (int *) malloc(1+str_length * sizeof(int));
        for(int j=0;j<str_length;j++){
            matrixgraph[ii-1][j]=0;
        }
        int o = 2;
        for (j = 0; grafy->data[i][j] != '\0'; j++) {                
            if (isdigit(grafy->data[i][j])){
                *line = grafy->data[i][j];
                x =  (x*10 + *line - '0' );                
                matrixgraph[ii-1][o-1] = x ;                 
            }
            else { 
                x = 0;               
                o++;
                 matrixgraph[ii-1][0] = o - 3;
            }
        }
        free(line);
    }
    return matrixgraph;
    
}
void rearrEdges(point* P,int oldDegree){
    for(int i=0;i<P->degree;i++){
        if(P->edges[i]==NULL){
            while(P->edges[oldDegree-1]==NULL && oldDegree>i){
                oldDegree--;
            }
            P->edges[i]=P->edges[oldDegree-1];
            P->edges[oldDegree-1]=NULL;
            oldDegree--;
        }
    }
    if(oldDegree*4<P->edgesmemory){
        P->edgesmemory /=2;
        point **newEdges=realloc(P->edges,sizeof(point)*P->edgesmemory);
        //check if realloced properly
        if(P->edges==NULL){
            printf("Realloc ERROR\n");
            exit(-1);
        }
        P->edges=newEdges;
    }
}
void addEdge(point* P1,point* P2){
    if(P1->edgesmemory==P1->degree){
        P1->edgesmemory *=2;
        point **newEdges=realloc(P1->edges,sizeof(point)*P1->edgesmemory);
        //check if realloced properly
        if(P1->edges==NULL){
            printf("Realloc ERROR\n");
            exit(-1);
        }
        P1->edges=newEdges;
    }
    P1->edges[P1->degree]=P2;
    P1->degree++;
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
    if(P1->degree!=actDegree){
        rearrEdges(P1,actDegree);
    }
}
bool haveEdge(point* P1,point* P2){
    //return 1 when edges are added 0 otherwise
    bool haveEdge=false;
    int i=0;
    while(!haveEdge&&i<P1->degree){
        if(P1->edges[i]==P2){
            haveEdge=true;
        }
        i++;
    }
    return haveEdge;
}
void tieEdge(point* P1,point* P2){
    if(!haveEdge(P1,P2)){
        addEdge(P1,P2);
        addEdge(P2,P1);
    }
}
void rmPoint(graph* G,int i){
    if(G->points[i]->degree>0){
        rmEdge(G->points[i]->edges[0],G->points[i]);
    }
    if(G->points[i]->degree>1){
        for(int j=1;j<G->points[i]->degree;j++){
            rmEdge(G->points[i]->edges[j],G->points[i]);
            tieEdge(G->points[i]->edges[j-1],G->points[i]->edges[j]);
        }
    }
    free(G->points[i]->edges);
    free(G->points[i]);
    G->p_sum--;
    G->points[i]=G->points[G->p_sum];
    G->points[G->p_sum]=NULL;
}
void rmExcessPoints(graph* G){
    int i=0;
    while(i<G->p_sum&&G->p_sum>4){
        if(G->points[i]->degree<3){
            rmPoint(G,i);
            i=-1;
        }
        i++;
    }
}
void rmExcessEdges(graph* G){
    for(int i=0;i<G->p_sum;i++){
        rmEdge(G->points[i],G->points[i]);
    }
    int *edges=(int*)malloc(sizeof(int)*G->p_sum);
    if(edges==NULL){
        printf("Malloc ERROR\n");
        exit(-1);
    }
    for(int j=0;j<G->p_sum;j++){
        for(int i=0;i<G->p_sum;i++){
            edges[i]=0;
        }
        for(int k=0;k<G->points[j]->degree;k++){
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
    if(G->points==NULL){
        freeGraph(G);
        free(G);
        freeParsed(matrixGraph);
        printf("Malloc ERROR\n");
        exit(-1);
    }
    for(int i=0;i<G->p_sum;i++){
        G->points[i]=(point *)malloc(sizeof(point));
        if(G->points[i]==NULL){
            freeGraph(G);
            free(G);
            freeParsed(matrixGraph);
            printf("Malloc ERROR\n");
            exit(-1);
        }
        G->points[i]->degree=matrixGraph[i][0];
        G->points[i]->num=matrixGraph[i][1];
        G->points[i]->edges=(point **)malloc(sizeof(point)*G->points[i]->degree);
        if(G->points[i]->edges==NULL){
            freeGraph(G);
            free(G);
            freeParsed(matrixGraph);
            printf("Malloc ERROR\n");
            exit(-1);
        }
        G->points[i]->edgesmemory=G->points[i]->degree;
        for(int j=0;j<G->points[i]->degree;j++){
            G->points[i]->edges[j]=G->points[matrixGraph[i][j+2]-1];
        }
    }
    for(int i=0;i<G->p_sum;i++){
        for(int j=0;j<G->points[i]->degree;j++){
            G->points[i]->edges[j]=G->points[matrixGraph[i][j+2]-1];
        }
    }
    freeParsed(matrixGraph);
}
point **getSame(point** P1,point** P2,int *count,int i,int k){
    point** connections=malloc(sizeof(point*)*k);
    if(connections==NULL){   
        printf("Malloc ERROR\n");
        exit(-1);
    }
    for(int j=0;j<k;j++){
        connections[j]=NULL;
    }
    for(int p=0;p<i;p++){
        for(int ii=0;ii<k;ii++){
            if(P1[p]==P2[ii]){
                connections[*count]=P1[p];
                (*count)++;
            }
        }
    }
    return connections;
}
point **matchPoints(point** connect1,point** connect2,int c1,int *c2,point* P,int l,int *size,bool method){
    bool pointB=false;
    int k=0;
    int newCount=0;
    int addint=0;
    for(int j=0;j<*c2;j++){
        if(P==connect2[j]){
            point* tmp=connect2[k];
            connect2[k]=connect2[j];
            connect2[j]=tmp;
            tmp=NULL;
            if(method){
                addint=1;
                k++;
                newCount++;
            }
            pointB=true;
        } 
    }
    if(pointB){
        for(int i=addint;i<*c2;i++){
            for(int j=0;j<c1;j++){
               if(connect1[j]==connect2[i]){
                   connect2[k]=connect1[j];
                   k++;
                   newCount++;
               } 
            }
        }
        *c2=newCount;
        if(*size+1>=l){
            point** match=NULL;
            match=malloc(sizeof(point*)*l*2);
            if(match==NULL){
                printf("Malloc ERROR\n");
                exit(-1);
            }
            for(int i=0;i<l;i++){
                match[i]=NULL;
            }
            for(int i=0;i<*c2;i++){
                match[i]=connect2[i];
            }
            free(connect2);
            connect2=match;
            match=NULL;
            *size = 2*l;
        }
    }
    else{
        *c2=0;
    }
    return connect2;
}
void solvegraph(Array *grafy,int *beginGraphs,int Gcount){
    graph *Graph=malloc(sizeof(graph));
    for(int index=0;index<Gcount;index++){
        int s=1;
        int rowCount=0;
        if(index+1==Gcount){
            rowCount=grafy->pocetriadkov-beginGraphs[index];
        }
        else{
            rowCount=beginGraphs[index+1]-beginGraphs[index];
        }
        int **matrixGraph=parseGraph(grafy,beginGraphs,Gcount,index,rowCount);
        initGraph(Graph,matrixGraph,rowCount-1);
        int j=0;
        if(Graph->p_sum>4){
            rmExcessEdges(Graph);
            checkEdgePoints(Graph);
            rmExcessPoints(Graph);
        }
        //Solve is there K33 or K5?
        int con=Graph->p_sum;
        int *connected=malloc(sizeof(int)*Graph->p_sum);
        for(int i=0;i<Graph->p_sum;i++){
            connected[i]=0;
        }
        point ***connectednum=(point***)malloc(sizeof(point)*Graph->p_sum);
        for(int i=0;i<Graph->p_sum;i++){
            connectednum[i]=(point**)malloc(sizeof(point)*Graph->p_sum);
            for(int m=0;m<Graph->p_sum;m++){
                connectednum[i][m]=NULL;
            }
        }
        bool end=false,foundK33=false;
        //K3,3 - must be before K5 because K5 removes edges/points
        //       that might delete K3,3 from graph
        j=Graph->p_sum;
        int k=0;
        bool method=false;
        while(Graph->p_sum>5&&!end){
            if(k<j){
                point* P1=Graph->points[k];
                for(int i=0;i<P1->degree;i++){
                    for(int l=0;l<P1->edges[i]->degree;l++){
                        point* P2=P1->edges[i]->edges[l];
                        int Count=0;
                        point** connections=NULL;
                        if(P2!=P1){
                            int size=P2->degree-1;
                            connections=getSame(P1->edges,P2->edges,&Count,P1->degree,P2->degree);
                            connections=matchPoints(P1->edges,connections,P1->degree,&Count,P1->edges[i],P1->degree,&size,method);
                        }
                        if(Count>2){
                            for(int m=0;m<P2->degree;m++){
                                if(P2->edges[m]!=P1->edges[i]){
                                    for(int n=0;n<P2->edges[m]->degree;n++){
                                        point* P3=P2->edges[m]->edges[n];
                                        int Count2=0;
                                        point** connections2=NULL;
                                        if((P2!=P3)&&(P1!=P3)){
                                            int size2=P3->degree-1;
                                            connections2=getSame(P2->edges,P3->edges,&Count2,P2->degree,P3->degree);
                                            connections2=matchPoints(connections,connections2,Count,&Count2,P2->edges[m],P2->degree,&size2,method);
                                            connections2=matchPoints(P2->edges,connections2,P2->degree,&Count2,P1->edges[i],P2->degree,&size2,method);
                                        }
                                        if(Count2>2){
                                            end=true;
                                            foundK33=true;
                                            s=0;
                                            n=P2->edges[m]->degree;
                                        }
                                        free(connections2);
                                    }
                                    if(end){
                                        m=P2->degree;
                                    }
                                }
                            }
                        }
                        free(connections);
                        if(end){
                            l=P1->edges[i]->degree;
                        }
                    }
                    if(end){
                        i=P1->degree;
                    }
                }
                if(P1->degree<4){     
                    rmPoint(Graph,k);
                    k=-1;
                    j--;
                }
            }
            else{
                end=true;
            }
            k++;
        }
        if(!foundK33){
            end=false;
        }
        //K5
        k=0;
        method=true;
        while(Graph->p_sum>4&&!end){
            bool rm=false;
            if(k<j){
                for(int i=0;i<Graph->points[k]->degree;i++){
                    connected[i]=0;
                }
                int interconnected=0;
                int m=0;
                for(int i=0;i<Graph->points[k]->degree;i++){
                    m=0;
                    for(int l=0;l<Graph->points[k]->edges[i]->degree;l++){                        
                        if(haveEdge(Graph->points[k]->edges[i]->edges[l],Graph->points[k])){
                            connected[i]++;
                            connectednum[i][m]=Graph->points[k]->edges[i]->edges[l];
                            m++;
                        }
                    }
                    if(connected[i]>2){
                        interconnected++;
                    }
                }
                if(interconnected>3){
                    //check overlap in connectednum K5
                    for(int n=0;n<Graph->points[k]->degree;n++){
                        int prevCount=0;
                        point **prevconnection=getSame(Graph->points[k]->edges,connectednum[n],&prevCount,Graph->points[k]->degree,connected[n]);
                        if(prevCount>2){
                            for(int o=n+1;o<Graph->points[k]->degree-1;o++){
                                int prevCount2=0;
                                int prevSize2=connected[o];
                                point **prevconnection2=getSame(Graph->points[k]->edges,connectednum[o],&prevCount2,Graph->points[k]->degree,connected[o]);
                                prevconnection2=matchPoints(prevconnection,prevconnection2,prevCount,&prevCount2,Graph->points[k]->edges[n],Graph->points[k]->degree,&prevSize2,method);
                                if(prevCount2>2){
                                    for(int p=o+1;p<Graph->points[k]->degree;p++){
                                        int prevCount3=0;
                                        int prevSize3=connected[p];
                                        point **prevconnection3=getSame(Graph->points[k]->edges,connectednum[p],&prevCount3,Graph->points[k]->degree,connected[p]);
                                        prevconnection3=matchPoints(prevconnection2,prevconnection3,prevCount2,&prevCount3,Graph->points[k]->edges[o],Graph->points[k]->degree,&prevSize3,method);
                                        prevconnection3=matchPoints(prevconnection3,prevconnection3,prevCount3,&prevCount3,Graph->points[k]->edges[n],Graph->points[k]->degree,&prevSize3,method);
                                        if(prevCount3>2){
                                            for(int i=p+1;i<Graph->points[k]->degree;i++){
                                                int prevCount4=0;
                                                int prevSize4=connected[i];
                                                point **prevconnection4=getSame(Graph->points[k]->edges,connectednum[i],&prevCount4,Graph->points[k]->degree,connected[i]);
                                                prevconnection4=matchPoints(prevconnection3,prevconnection4,prevCount3,&prevCount4,Graph->points[k]->edges[p],Graph->points[k]->degree,&prevSize4,method);
                                                prevconnection4=matchPoints(prevconnection4,prevconnection4,prevCount4,&prevCount4,Graph->points[k]->edges[o],Graph->points[k]->degree,&prevSize4,false);                                               
                                                prevconnection4=matchPoints(prevconnection4,prevconnection4,prevCount4,&prevCount4,Graph->points[k]->edges[o],Graph->points[k]->degree,&prevSize4,false);                                               
                                                if(prevCount4>2){
                                                    s=0;
                                                    end=true;
                                                    p=n=o=Graph->points[k]->degree;
                                                    //found K5
                                                }
                                                else if(i+1==Graph->points[k]->degree && p+2==Graph->points[k]->degree && o+3==Graph->points[k]->degree && n+4==Graph->points[k]->degree){                                            
                                                    rm=true;
                                                }
                                                free(prevconnection4);
                                            }
                                        }
                                        free(prevconnection3);
                                    }    
                                }                           
                                free(prevconnection2);
                            }
                        }
                        free(prevconnection);
                    }
                    if(rm){
                        rmPoint(Graph,k);
                        k=-1;
                        j--;    
                    }
                }
                else{
                    rmPoint(Graph,k);
                    k=-1;
                    rm=true;
                    j--;
                }
            }
            else{
                end=true;
            }
            if(!rm){
                if(Graph->points[k]->degree<4){
                    rmPoint(Graph,k);
                    k=-1;
                    j--;
                }
            }
            k++;            
        }
        for(int i=0;i<con;i++){
            free(connectednum[i]);
        }
        free(connectednum);
        free(connected);
        freeGraph(Graph);
        char *answer[2]={"non-planar","planar"};
        fprintf(stdout,"Graph #%d is %s\n",index+1,answer[s]);
    }
    free(Graph);
}
