#ifndef DIJK_H
#define DIJK_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

size_t find_min_distance(size_t *distances, bool *visited, size_t order);

size_t* dijkstra(struct graph *g, size_t source, size_t dest, size_t *path_length);

#endif