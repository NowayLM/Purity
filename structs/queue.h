#ifndef QUEUE_H
#define QUEUE_H
#include <stddef.h>

struct queue {
    size_t *arr;
    size_t size;
    int Rear;
    int Front;
};


struct queue initQueue(size_t size);
void enqueue(struct queue queue, size_t elm);
size_t dequeue(struct queue queue);
int isempty(struct queue queue);

#endif