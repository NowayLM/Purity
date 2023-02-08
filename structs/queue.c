#include <stdio.h>
#include <string.h>
#include "queue.h"
#include <stdlib.h>



struct queue *initQueue(size_t size) {
    struct queue *queue = calloc(1, sizeof(struct queue));
    queue->arr = calloc(size, sizeof(size_t));
    for(size_t i = 0; i < size; i++)
        queue->arr[i] = queue->size;
    queue->Front = 0;
    queue->Rear = 0;
    queue->size = size;
    return queue;
}

//pour une raison obscure le queue.arr résultant de initQueue n'est pas le bon

void enqueue(struct queue *queue, size_t elm){
    if ((queue->Rear + 1) == queue->size)
        queue->Rear = 0;
    else
        queue->Rear++;
    printf("queue.Rear = %zu\n", queue->Rear);
    printf("queue.arr[queue.front] = %zu\n", queue->arr[queue->Front]);
    if (queue->arr[queue->Rear] != queue->size) {
        printf("queue overflow\n");
        return;
    }
    printf("ALL GOOD WTFFFF\n");
    //queue.Rear = ((queue.Rear + 1)%queue.size);
    printf("queue rear = %i\n", queue->Rear);
    queue->arr[queue->Rear] = elm;
}

size_t dequeue(struct queue *queue){
    size_t res = queue->arr[queue->Front];
    queue->arr[queue->Front] = queue->size;
    if((queue->Front + 1) == queue->size)
        queue->Front = 0;
    else
        queue->Front++;
    return res;
}

int isempty(struct queue *queue) {
    printf("queue.size = %zu\n", queue->size);
    printf("queue.arr[queue.front] = %zu\n", queue->arr[queue->Front]);
    if (queue->arr[queue->Front] == queue->size)
        return 1;
    return 0;
}
