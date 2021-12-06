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
        //    fprintf(stdout,"----%d.---- >/", str_length);
        int *riadok = (int *) malloc (str_length * sizeof (int));
        matrixgraph[ii-1] = (int *) malloc(1+str_length * sizeof(int));
        for(int j=0;j<str_length;j++){
            matrixgraph[ii-1][j]=0;
        }
        int o = 2;
        //int matrixgraph[i][str_length];

        for (j = 0; grafy->data[i][j] != '\0'; j++) {
                
            if (isdigit(grafy->data[i][j])){
                //fprintf(stdout,"na3iel som cislo>%d", grafy->data[i][j]);
                *riadok = grafy->data[i][j];

                x =  (x*10 + *riadok - '0' );
                //fprintf(stdout,"rozsirujem cislo>%d \n", x); 
                
                matrixgraph[ii-1][o-1] = x ;
                //fprintf(stdout,"vkladam cislo>%d na %dx%d\n", x,i,o-1);                     
            }
            else { 

                x = 0;
               
                o++;
                 matrixgraph[ii-1][0] = o - 3;
            }

        }
        free(riadok);
    }
    int a,b;
    //printf("\n");
    for(a=0;a<rowcount-1;a++)
     {
          
          for(b=0;b<= matrixgraph[a][0]+1;b++)
          {
               //printf("%3d ",matrixgraph[a][b]);
               //Here, %3d takes 3 digit space for each digit while printing  output 
          }
          //printf("\n");

    }
    return matrixgraph;
    
}
void rearrEdges(point* P,int oldDegree){
    //no malloc!
    /*point** newEdges=(point **)malloc(sizeof(point)*P->degree);
    for(int i=0;i<P->degree;i++){
        newEdges[i]=NULL;
    }
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
    }*/
    //new rearredges
    //printf("OldeDeg:NewDeg:: %d|%ld\n",oldDegree,P->degree);
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
            printf("realloc ERROR\n");
            exit(1);
        }
        P->edges=newEdges;
    }
}
void addEdge(point* P1,point* P2){
    //reduce malloc
    /*point** newEdges=malloc(sizeof(point)*(P1->degree+1));
    for(int i=0;i<P1->degree;i++){
        newEdges[i]=P1->edges[i];
    };
    newEdges[P1->degree]=P2;
    point** tmp=P1->edges;
    P1->edges=newEdges;
    P1->degree++;
    free(tmp);*/
    //new addEdge
    if(P1->edgesmemory==P1->degree){
        P1->edgesmemory *=2;
        point **newEdges=realloc(P1->edges,sizeof(point)*P1->edgesmemory);
        //check if realloced properly
        if(P1->edges==NULL){
            printf("realloc ERROR\n");
            exit(1);
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
    int *edges=(int*)malloc(sizeof(int)*G->p_sum);
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
    for(int i=0;i<G->p_sum;i++){
        G->points[i]=(point *)malloc(sizeof(point));
        G->points[i]->degree=matrixGraph[i][0];
        G->points[i]->num=matrixGraph[i][1];
        G->points[i]->edges=(point **)malloc(sizeof(point)*G->points[i]->degree);
        G->points[i]->edgesmemory=G->points[i]->degree;
        for(int j=0;j<G->points[i]->degree;j++){
            G->points[i]->edges[j]=G->points[matrixGraph[i][j+2]-1];
        }
        if(i+1!=G->p_sum){
            G->points[i]->n_point=G->points[i+1];
        }
        else{
            G->points[i]->n_point=NULL;
        }
    }
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
    }
    G->first_p=G->points[0];
    freeParsed(matrixGraph);
}
point **getSame(point** P1,point** P2,int *count,int i,int k){
    point** connections=malloc(sizeof(point*)*k);
    for(int j=0;j<k;j++){
        connections[j]=NULL;
    }
    for(int p=0;p<i;p++){
        for(int ii=0;ii<k;ii++){
            if(P1[p]==P2[ii]){
                connections[*count]=P1[p];
                (*count)++;
            }
            //printf("P1[%d]:P2[%d],%ld:%ld\n",p,ii,P1[p]->num,P2[ii]->num);
        }
    }
    return connections;
}
point **matchPoints(point** connect1,point** connect2,int c1,int *c2,point* P,int l){
    bool pointB=false;
    int k=0;
    int newCount=0;
    point** match=malloc(sizeof(point*)*l);
    for(int j=0;j<*c2;j++){
        if(P==connect2[j]){
            match[k]=connect2[j];
            newCount++;
            pointB=true;
        } 
    }
    if(pointB){
        for(int i=0;i<c1;i++){
            for(int j=0;j<*c2;j++){
               if(connect1[i]==connect2[j]){
                   match[k]=connect1[i];
                   newCount++;
               } 
            }
        }
        *c2=newCount;
    }
    else{
        *c2=0;
    }
    free(connect2);
    return match;
}
void solvegraph(Array *grafy,int *beginGraphs,int Gcount){
    graph *Graph=(graph *)malloc(sizeof(graph));
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
        initGraph(Graph,matrixGraph,rowCount-1);//workaround nonexistent input
        
        // printf("Original no.%d\n",index+1);
        // for(int i=0;i<Graph->p_sum;i++){
            // printf("Point:%ld|",Graph->points[i]->num);
            // for(int j=0;j<Graph->points[i]->degree;j++){
                // if(Graph->points[i]->edges[j]!=NULL){
                    // printf("%ld ",Graph->points[i]->edges[j]->num);                
                // }
            // }
            // printf("\n");
        // }
        int *highDegIndex=malloc(sizeof(int)*Graph->p_sum);
        int j=0;
        if(Graph->p_sum>5){
            rmExcessEdges(Graph);
            checkEdgePoints(Graph);
            rmExcessPoints(Graph);
            //unnecesry? all points have deg>2 if left in graph
            for(int i=0;i<Graph->p_sum;i++){
                if(Graph->points[i]->degree>2){
                    highDegIndex[j]=i;
                    j++;
                }
            }
            j--;
            //if deg>2 connected to deg <3 --> imposible if previous correction is right
            int connected=0,k=j;
            bool repeat=false;
            while(k>=0){
                connected=0;
                for(int i=0;i<j;i++){
                    if(haveEdge(Graph->points[highDegIndex[k]],Graph->points[highDegIndex[i]])){
                        connected++;
                    }
                }
                //printf("connected:%d\n",connected);
                if(connected<3){
                    printf("rmpoint:%ld\n",Graph->points[highDegIndex[k]]->num);
                    rmPoint(Graph,highDegIndex[k]);
                    highDegIndex[k]=highDegIndex[j];
                    j--;
                    k++;
                    repeat=true;
                }
                k--;
                if(k<0 && repeat){
                    k=j;
                }
            }
        }
        // printf("\n");
        // printf("Not-Solved no.%d\n",index+1);
        // for(int i=0;i<Graph->p_sum;i++){
            // printf("Point:%ld|",Graph->points[i]->num);
            // for(int j=0;j<Graph->points[i]->degree;j++){
                // if(Graph->points[i]->edges[j]!=NULL){
                    // printf("%ld ",Graph->points[i]->edges[j]->num);                
                // }
            // }
            // printf("\n");
        // }
        //Solve is there K33 or K5?
        int k=0;
        int con=Graph->p_sum;
        int *connected=malloc(sizeof(int)*Graph->p_sum);
        for(int i=0;i<Graph->p_sum;i++){
            connected[i]=0;
        }
        point ***connectednum=malloc(sizeof(point***)*Graph->p_sum);
        for(int i=0;i<Graph->p_sum;i++){
            connectednum[i]=malloc(sizeof(point**)*Graph->p_sum);
        }

        while(Graph->p_sum>4&&s!=0){
            if(k<=j){
                //K3,3 must be before K5 if k5 removes edges/points
                //K5
                bool rm=false;
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
                            //printf("conectednum[%d][%d]:%ld\n",i,m,connectednum[i][m]->num);
                            m++;
                        }
                    }              
                    //dif for k33 if( >1)      
                    //printf("conected[%d]:%d\n",i,connected[i]);
                    if(connected[i]>2){
                        interconnected++;
                    }
                }
                //diff for k33 if( >5)
                if(interconnected>4){
                    //check overlap in connectednum K5
                    //use m only if array ??
                    for(int i=0;i<Graph->points[k]->degree;i++){
                        m=0;
                        int n=i;
                        if(n<Graph->points[k]->degree){
                            int prevCount=0;
                            point **prevconnection=getSame(Graph->points[k]->edges,connectednum[n],&prevCount,n,connected[n]);
                            //printf("prevcount:%d\n",prevCount);
                            if(prevCount>0){
                                //point **sameconnection=getSame();
                                //2 rovnake +predosly 1
                                int o=n+1;
                                if(o<Graph->points[k]->degree){
                                    //1 rovnaky + 2 predosle
                                    int prevCount2=0;
                                    point **prevconnection2=getSame(Graph->points[k]->edges,connectednum[o],&prevCount2,o,connected[o]);
                                    // printf("Point:%ld Edge:%ld\n",Graph->points[k]->num,Graph->points[k]->edges[o]->num);
                                    // printf("PrevPoint:%ld",Graph->points[k]->edges[n]->num);
                                    // printf("prevcount21:%d\n",prevCount2);
                                    //check if prevconnections match at least 1 match + num of prev point
                                    prevconnection2=matchPoints(prevconnection,prevconnection2,prevCount,&prevCount2,Graph->points[k]->edges[n],Graph->points[k]->degree);
                                    // printf("prevcount22:%d\n",prevCount2);
                                    if(prevCount2>1){
                                        int p=o+1;
                                        if(p<Graph->points[k]->degree){
                                            //3 predosle
                                            int prevCount3=0;
                                            point **prevconnection3=getSame(Graph->points[k]->edges,connectednum[p],&prevCount3,p,connected[p]);
                                            // printf("Point:%ld Edge:%ld\n",Graph->points[k]->num,Graph->points[k]->edges[p]->num);
                                            // printf("PrevPoint:%ld",Graph->points[k]->edges[o]->num);
                                            // printf("prevcount31:%d\n",prevCount3);
                                            //check if prevconnections match at least 1 match + num of prev point
                                            prevconnection3=matchPoints(prevconnection2,prevconnection3,prevCount2,&prevCount3,Graph->points[k]->edges[o],Graph->points[k]->degree);
                                            // printf("prevcount32:%d\n",prevCount3);
                                            if(prevCount3>2){
                                                //printf("K5 found\n");
                                                s=0;
                                                //found K5
                                            }
                                            else{
                                                rm=true;
                                            }
                                            free(prevconnection3);
                                        }    
                                    }
                                    free(prevconnection2);
                                }
                            }
                            free(prevconnection);
                        }
                    }
                    //printf("K5");
                    if(rm){
                        rmPoint(Graph,k);
                        // printf("\n");
                        // printf("Not-Solved no.%d\n",index+1);
                        // for(int i=0;i<Graph->p_sum;i++){
                            // printf("Point:%ld|",Graph->points[i]->num);
                            // for(int j=0;j<Graph->points[i]->degree;j++){
                                // if(Graph->points[i]->edges[j]!=NULL){
                                    // printf("%ld ",Graph->points[i]->edges[j]->num);                
                                // }
                            // }
                            // printf("\n");
                        // }
                        k=-1;
                        j--;
                        //printf("---------again---------");
    
                    }
                }
                else{
                    rmPoint(Graph,k);
                    // printf("\n");
                    // printf("Not-Solved no.%d\n",index+1);
                    // for(int i=0;i<Graph->p_sum;i++){
                        // printf("Point:%ld|",Graph->points[i]->num);
                        // for(int j=0;j<Graph->points[i]->degree;j++){
                            // if(Graph->points[i]->edges[j]!=NULL){
                                // printf("%ld ",Graph->points[i]->edges[j]->num);                
                            // }
                        // }
                        // printf("\n");
                    // }
                    k=-1;
                    j--;
                    //printf("---------again---------");
                }
                //printf("\n");
            }
            else{
                s=0;
            }
            k++;
            
        }
        for(int i=0;i<con;i++){
            free(connectednum[i]);
        }
        free(connectednum);
        free(connected);
        free(highDegIndex);
        // printf("\n");
        // printf("Solved no.%d\n",index+1);
        // for(int i=0;i<Graph->p_sum;i++){
            // printf("Point:%ld|",Graph->points[i]->num);
            // for(int j=0;j<Graph->points[i]->degree;j++){
                // if(Graph->points[i]->edges[j]!=NULL){
                    // printf("%ld ",Graph->points[i]->edges[j]->num);                
                // }
            // }
            // printf("\n");
        // }
        
        freeGraph(Graph);
        char *answer[2]={"non-planar","planar"};
        fprintf(stdout,"Graph #%d is %s\n",index+1,answer[s]);
    }
    free(Graph);
}
