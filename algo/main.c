#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../structs/graph.h"
#include "../structs/queue.h"

void freeAll(struct graph *G, struct queue *queue) {
    freeGraph(G);
    freeQueue(queue);
}

int main(){
    /*printf("Please enter the filepath for the map.\n");
    char filepath[100];
    scanf("%s", filepath);*/
    char *filepath = "maps/1.txt";
    struct graph *G = buildGraph(filepath);
    struct queue *queue = initQueue(G->order);
    freeAll(G, queue);
}
