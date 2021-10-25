/*
*
* @author Ľubomír Závodský <xzavod14@vutbr.cz>
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
typedef struct {
    char **data;
    size_t pocetriadkov;
    size_t velkostpola;

} Array;
//size_t is unsigned integer data type. On systems using the GNU C Library, this will be unsigned int or unsigned long int. size_t is commonly used for array indexing and loop counting. size_t or any unsigned type might be seen used as loop variable as loop variables are typically greater than or equal to 0.
//funkcie na tvorbu nafukovacieho pola:

//funkce na inicializaciu pola znakov

char* dynamic_fgets(char **bufferpointer, size_t *sizepointer, FILE *fp);
//dynamicky nacita jeden riadok stringu ktory moze byt nekonecne dlhy
void initArray(Array *array, size_t pocvelkost);
/*inicializacia pola  pre zadanu velkost*/
void insertstring(Array *array, char *string);
/*vkladanie prvkov do pola s dynamickym rozširovaním*/
void freeArray(Array *array);
