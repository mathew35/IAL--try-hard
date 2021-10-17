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
//#include "script.h"
#include <stdio.h>
#include <time.h>
int main(int argc, char *argv[]) {
  //kontrola rargumentov
  char c;
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






  }


    //pripadne argumenty pre debug a podobne
  else{

    printf("Nacitam subor: %s\n", argv[1]);
    if ((file = fopen(argv[1], "r")) == NULL)
    {
       printf("Soubor se nepodarilo otevrit\n");
       return 0;
    }

    while (c != EOF) {
      c = getc(file);
      printf("%c",c);
    }

  }

  fclose(file);
   return 0;

}
