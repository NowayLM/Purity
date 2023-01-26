#include <stdio.h>
#include <string.h>
#include "../structs/utils.h"

/* this is the test file for the algorithm part

*/

int main(){
    printf("Please enter the filepath for the map.\n");
    char filepath[100];
    scanf("%s", filepath);
    struct graph *G = buildGraph(filepath);
    freeGraph(G);
}