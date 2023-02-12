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
    /*if (queue->Rear == queue->Front) {
        errx(3, "enqueue(queue) : queue overflow");
        return;
    }*/
    queue->arr[queue->Rear] = elm;
    if ((queue->Rear + 1) == queue->size)
        queue->Rear = 0;
    else
        queue->Rear++;
}

size_t dequeue(struct queue *queue){
    if (isempty(queue) == 1)
        errx(3, "dequeue(queue) : queue underflow");
    size_t res = queue->arr[queue->Front];
    queue->arr[queue->Front] = 0;
    if((queue->Front + 1) == queue->size)
        queue->Front = 0;
    else
        queue->Front++;
    return res;
}

int isempty(struct queue *queue) {
    if (queue->Front == queue->Rear)
        return 1;
    return 0;
}

void freeQueue(struct queue *queue)
{
    if (queue != NULL) {
        free(queue->arr);
        free(queue);
    }
}

void printQueueFR(struct queue *queue) {
    printf ("front = %zu,  rear = %zu,  queue = [", queue->Front, queue->Rear);
    if (isempty(queue) == 0) {
        for(size_t i = queue->Front; i < queue->Rear - 1; i++)
            printf("%zu, ", queue->arr[i]);
        printf("%zu]\n", queue->arr[queue->Rear - 1]);
    }
    else
        printf("]\n");
}


void printQueue(struct queue *queue) {
    printf ("queue = [");
    if (isempty(queue) == 0) {
        for(size_t i = queue->Front; i < queue->Rear - 1; i++)
            printf("%zu, ", queue->arr[i]);
        printf("%zu]\n", queue->arr[queue->Rear - 1]);
    }
    else
        printf("]\n");
}

/* printf ("front = %zu,  rear = %zu,  queue = [", queue->Front, queue->Rear);
    size_t i = queue->Front;
    if (queue->Rear != 0) {
        while (i != queue->Rear - 1) {
            if((i + 1) == queue->size)
                i = 0;
            else
                i++;
            printf("%zu, ", queue->arr[i]);
        }
        printf("%zu]\n", queue->arr[queue->Rear - 1]);
    }
    while (i != queue->size) {
        printf("%zu, ", queue->arr[i]);
        if((i + 1) == queue->size)
            i = 0;
        else
        i++;
        
    }
        printf("%zu]\n", queue->arr[queue->Rear - 1]);*/