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
    for (size_t i = 0; i < 100; i++)
    {
        enqueue(queue, i);
        printf("front = %zu    rear = %zu\n", queue->Front, queue->Rear);
        printf("dequeued elm = %zu\n", dequeue(queue));
    }
    freeGraph(G);
    free(queue->arr);
    free(queue);
}
