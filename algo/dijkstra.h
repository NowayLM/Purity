#ifndef DIJK_H
#define DIJK_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

size_t find_min_distance(size_t *distances, bool *visited, size_t order);

size_t euclidean_distance(size_t x1, size_t y1, size_t x2, size_t y2);

//size_t* dijkstra(struct graph *g, size_t source, size_t dest, size_t *path_length);

size_t *dijkstra(struct graph *g, size_t start, size_t end, size_t *path_length);

#endif
