#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "../structs/graph.h"
#include "../structs/queue.h"

void freeAll(struct graph *G, size_t *path) {
    freeGraph(G);
    free(path);
}

size_t __ladder(struct graph *G, size_t end, size_t start, size_t *L) {
    L[end] = G->order + 1;
    struct queue *queue = initQueue(G->order);
    enqueue(queue, end);
    size_t x;
    while (isempty(queue) == 0) {
        x = dequeue(queue);
        for (size_t i = 0; i < G->inters[x].nblinks; i++) {
            size_t adj = G->inters[x].links[i].end;
            if (adj == start) {
                L[adj] = x;
                freeQueue(queue);
                return adj;
            }
            if (L[adj] == G->order) {
                enqueue(queue, adj);
                L[adj] = x;
            }
            
        }
        
    }
    err(3, "target place wasn't in the graph");
}

size_t *ladder(struct graph *G, size_t start, size_t end) {
    size_t *L = calloc(G->order, sizeof(size_t));
    for (size_t i = 0; i < G->order; i++)
        L[i] = G->order;
    size_t *res = calloc(G->order, sizeof(size_t));
    __ladder(G, end, start, L);
    size_t x = start;
    size_t i = 1;
    res[0] = start;
    while (x != G->order + 1){
        res[i] = L[x];
        x = L[x];
        i++;
    }
    res[i - 1] = G->order;
    res = realloc(res, (i)*sizeof(size_t));
    return res;
}

int main(){
    /*printf("Please enter the filepath for the map.\n");
    char filepath[100];
    scanf("%s", filepath);*/
    printf("\n\n##########\n\nWelcome to Purity, the best guidance system.\n\n##########\n\n");
    char *filepath = "maps/1.txt";
    struct graph *G = buildGraph(filepath);
    printf("Enter the starting point and the destination, they must belong to [0 ; %zu] (format : start dest)\n\n", G->order);
    size_t start;
    size_t end;
    scanf("%zu %zu", &start, &end);
    if (start == end)
        errx(3, "start point can't be destination");
    if (start >= G->order || end >= G->order)
        errx(3, "start point and dest must be less than %zu.\n", G->order);
    printf("\n\nComputing path from %zu to %zu.\n\n", start, end); 
    size_t *path = ladder(G, start, end);
    size_t i = 0;
    printf("[");
    while(path[i] != end) {
      printf("%zu, ", path[i]);
      i++;
    }
    printf("%zu]\n\n", path[i]);
    freeAll(G, path);
    printf("Thank you for choosing Purity to guide you once again.\n");
    return 1;
}
