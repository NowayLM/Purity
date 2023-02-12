#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "../structs/graph.h"
#include "../structs/queue.h"



int main(){
    struct queue *queue = initQueue(10);
    printQueue(queue);
    enqueue(queue, 10);
    printQueue(queue);
    enqueue(queue, 20);
    printQueue(queue);
    enqueue(queue, 30);
    printQueue(queue);
    printf("%zu\n", dequeue(queue));
    enqueue(queue, 40);
    printQueue(queue);
    enqueue(queue, 50);
    printQueue(queue);
    printf("%zu\n", dequeue(queue));
    printQueue(queue);
    printf("%zu\n", dequeue(queue));
    printQueue(queue);
    printf("%zu\n", dequeue(queue));
    printQueue(queue);
    printf("%zu\n", dequeue(queue));
    printQueue(queue);
    printf("%zu\n", dequeue(queue));
    printQueue(queue);
    printf("if this shows then something's wrong");
    freeQueue(queue);
    return 1;
}