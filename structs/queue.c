#include <stdio.h>
#include <string.h>
#include "queue.h"



struct queue initQueue(size_t size) {
    struct queue queue;
    size_t tmp[size];
    for(size_t i = 0; i < size; i++)
        tmp[i] = queue.size;
    queue.arr = tmp;
    queue.Front = 0;
    queue.Rear = 0;
    queue.size = size;
    return queue;
}

//pour une raison obscure le queue.arr résultant de initQueue n'est pas le bon

void enqueue(struct queue queue, size_t elm){
    printf("queue front = %i\n", queue.Front);
    printf("queue rear = %i\n", queue.Rear);
    size_t tmp = ((queue.Rear + 1)%queue.size);
    printf("queue arr[0] = %zu\n", queue.arr[0]);
    printf("queue arr[tmp] = %zu\n", queue.arr[tmp]);
    if (queue.arr[tmp] != queue.size) {
        printf("queue overflow\n");
        return;
    }
    queue.Rear = ((queue.Rear + 1)%queue.size);
    printf("queue rear = %i\n", queue.Rear);
    queue.arr[queue.Rear] = elm;
}

size_t dequeue(struct queue queue){
    size_t res = queue.arr[queue.Front];
    queue.arr[queue.Front] = queue.size;
    queue.Front = ((queue.Front + 1)%queue.size);
    return res;
}

int isempty(struct queue queue) {
    if (queue.arr[queue.Front] == queue.size)
        return 1;
    return 0;
}