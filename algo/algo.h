#ifndef ALGO_H
#define ALGO_H
#include <stddef.h>

void freeAll(struct graph *G, size_t *path); //frees a graph* G and a size_t* path

size_t __ladder(struct graph *G, size_t end, size_t start, size_t *L); //recursive call for the BFS

size_t *ladder(struct graph *G, size_t start, size_t end); //call function for the BFS

#endif
