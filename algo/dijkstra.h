#ifndef DIJK_H
#define DIJK_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

size_t cost (size_t x, size_t y, struct graph *G);

size_t euclidean_distance(double x1, double y1, double x2, double y2);

size_t compute_path_length (size_t path_length, size_t *path, struct graph *G);

size_t *dijkstra(struct graph *g, size_t start, size_t end, size_t *path_length);

#endif
