/*
** Projekt do předmětu Algoritmy                     **
* Varianta 5                                          *
* Autori:  Lubomír Závodský <xzavod14>				  *
*                                                     *
*                                                     *
*                                                     *
* rok vytvoření: 2021		                          *
* projekt ial 2021/2022                               *
* Program funguje jako konzolová aplikace, ktorá načíta grafy zo souboru a následne vyhodnocuje či sa ich hrany pretinaju*
*/
//#include "array.h"
#include "solution.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  //pole testing

  size_t linelen = 100;
  //free this char *line at the end!!!
  char *line = malloc(linelen);
  Array *grafy = NULL;
  grafy = malloc(sizeof(Array));
  if (grafy == NULL) {
      printf("Nepodarilo sa vyhradit miesto (malloc error)\n");
      return -1;
  }
  initArray(grafy, 1); /* 1 riadok */
  FILE * file;
  if (argc == 1)
  {
     printf("Neni zadan vstupni soubor\n");
     printf("Skus ./graph -help\n");
     return 0;
  }
  if (strcmp(argv[1],"-help")==0)
  {
    printf("Help ku aplikaci na kontrolu rovinnosti graphu *TODO\n");
    return 0;
  }
  else if (argc == 2)
  {
    //nacitanie
    //konrola zda ide otvorit subor
    if ((file = fopen(argv[1], "r")) == NULL)
    {
       printf("Soubor se nepodarilo otevrit\n");
       return 0;
    }

    while (dynamic_fgets(&line, &linelen, file) != NULL) {
    //fprintf(stdout,"vkladam riadok:%s",c);
    insertstring(grafy, line);
    }
    for (int i = 0; i < grafy->pocetriadkov; ++i){
      //fprintf(stdout,"%d. :", i);
     // fprintf(stdout,"%s", grafy->data[i]);
    }
      fclose(file);
    solvegraph(grafy);//*grafy as input later on
  }
    //pripadne argumenty pre debug a podobne
  else{
    //deklarace>

    printf("Nacitam subor: %s\n", argv[1]);
    if ((file = fopen(argv[1], "r")) == NULL)
    {
       printf("Soubor se nepodarilo otevrit\n");
       return 0;
    }
    char c;
    c = fgetc(file);
    // kontrola prveho grafu
    if (c == '#'){    printf("Nacitam nazov: %c\n", c);
    //fseek(file, -1L, SEEK_CUR);
    dynamic_fgets(&line, &linelen, file);
     // printf("%s\n", line);
      c = fgetc(file);
      if (c == '#'){ printf("Prazdne grafy nebereme\n"); return 0;}
      fseek(file, -1L, SEEK_CUR);
    }
    else  {printf("Nazov musi zacinat s # nie s %c\n", c);
    return 0;
    }
    //seek(file_ptr, -1L, SEEK_CUR);
    while (dynamic_fgets(&line, &linelen, file) != NULL) {

    fprintf(stdout,"kontrolujem riadok:%s",line);

      fprintf(stdout,"vkladam riadok:%s",line);
      insertstring(grafy, line);
      printf("velkost pola:%ld\n", grafy->velkostpola);
      c = fgetc(file);
      fprintf(stdout,"kontrolujem prve pismeno:%c",c);
      if (c == '#'){
      //fseek(file, -1L, SEEK_CUR);
      dynamic_fgets(&line, &linelen, file);
       printf("Nacitam nazov: %s\n", line);
      break;
     }
      else{ printf("Na riadku nieje nazov: %s\n", line);
        fseek(file, -1L, SEEK_CUR);}
    }
        fclose(file);//subor precitany
     // fprintf(stdout,"toto je ulozene:");
        // automatically resizes as necessary
         //   fprintf(stdout,"%s ", grafy->data[0]);
        for (int i = 0; i < grafy->pocetriadkov; ++i){
          fprintf(stdout,"%d. >", i);
          //fprintf(stdout,"%s", grafy->data[i]);
        }
      printf("pocet pouzitych%ld\n", grafy->pocetriadkov);
  }
  //upratovanie  pameti
  freeArray(grafy);
  free(line);
   return 0;

}
