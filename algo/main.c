#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../structs/graph.h"
#include "../structs/queue.h"

int main(){
    /*printf("Please enter the filepath for the map.\n");
    char filepath[100];
    scanf("%s", filepath);*/
    char *filepath = "maps/1.txt";
    struct graph *G = buildGraph(filepath);
    struct queue *queue = initQueue(G->order);
    size_t elm = 2;
    printf("inserting element : %zu\n", elm);
    enqueue(queue, elm);
    size_t test = dequeue(queue);
    if (isempty(queue) == 1)
        printf("isempty = True\ninserted element was : %zu\n", test);
    printf("%zu\n", G->order);
    freeGraph(G);
    free(queue->arr);
}
