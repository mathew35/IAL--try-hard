#include <stdio.h>
#include "array.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
int test(){
  printf("test");
  return 0;
}
void initArray(Array *array, size_t pocvelkost) {
    array->data = malloc(pocvelkost * sizeof(char *));
    if (array->data == NULL) {
        printf("Nepodarilo sa alokvat!(malloc error)\n");
        exit(1);
    }
    array->pocetriadkov = 0;
    array->velkostpola = pocvelkost;
}

void insertstring(Array *array, char *string) {
    //exponencialne rozširenie pola vždy pri naplnení pola *2
    if (array->pocetriadkov == array->velkostpola) {
        void *pointer;
        array->velkostpola *= 2;
        pointer = realloc(array->data, array->velkostpola * sizeof(char *));
        if (array->data == NULL) {
            freeArray(array);
            printf("Nepodarilo sa alokvat!(malloc error)\n");
            exit(1);
        }
        array->data = pointer; // vkladame ukazatel na data
    }
    /* ak vkladany riadok neíeje null nakopiruj ho tam */
    if (string != NULL) {
        size_t length;

        length = strlen(string);
        array->data[array->pocetriadkov] = malloc(1 + length);
        if (array->data[array->pocetriadkov] != NULL)
            strcpy(array->data[array->pocetriadkov++], string);
    }
    else
        array->data[array->pocetriadkov++] = NULL;
}
char* dynamic_fgets(char **bufferpointer, size_t *sizepointer, FILE *fp) {
    size_t len;
    if(fgets(*bufferpointer, *sizepointer, fp) == NULL) return '\0';
    len = strlen(*bufferpointer);
    while(strchr(*bufferpointer, '\n') == NULL) {
        *sizepointer += 100;
        *bufferpointer = realloc(*bufferpointer, *sizepointer);
        if(fgets(*bufferpointer + len, *sizepointer - len, fp) == NULL) return  *bufferpointer;
        len += strlen(*bufferpointer + len);
    }
    return *bufferpointer;
}
void freeArray(Array *array) {
    size_t i;

    /* upracem vsetko v poli */
    for (i = 0; i < array->pocetriadkov; ++i){
        free(array->data[i]);
    }
    free(array->data);
    free(array); //preistotu aj ukazatel uvolnime
}
//
