#include <stdio.h>
#include <string.h>
#include <err.h>
#include "queue.h"
#include <stdlib.h>



struct queue *initQueue(size_t size) {
    struct queue *queue = calloc(1, sizeof(struct queue));
    queue->arr = calloc(size, sizeof(size_t));
    /*for(size_t i = 0; i < size; i++)
        queue->arr[i] = queue->size;*/
    queue->Front = 0;
    queue->Rear = 0;
    queue->size = size;
    return queue;
}

void enqueue(struct queue *queue, size_t elm){
    if ((queue->Rear + 1) == queue->size)
        queue->Rear = 0;
    else
        queue->Rear++;
    if (queue->Rear == queue->Front) {
        err(3, "queue overflow\n");
        return;
    }
    queue->arr[queue->Rear] = elm;
}

size_t dequeue(struct queue *queue){
    if((queue->Front + 1) == queue->size)
        queue->Front = 0;
    else
        queue->Front++;
    size_t res = queue->arr[queue->Front];
    queue->arr[queue->Front] = queue->size;
    return res;
}

int isempty(struct queue *queue) {
    if (queue->Front == queue->Rear)
        return 1;
    return 0;
}
