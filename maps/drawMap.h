#ifndef DRAWMAP_H
#define DRAWMAP_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../structs/graph.h"
#include "../algo/dijkstra.h"

void draw_vertex(SDL_Renderer *renderer, int x, int y, int radius);

int compute_pos(double x, int xOrY, size_t renderX, size_t renderY, size_t maxX, size_t cZoom, int graphOrNot, struct graph *G);

void draw_map(SDL_Renderer *renderer, struct graph *G, size_t *path, size_t pathLength, size_t maxX, size_t maxY, size_t renderX, size_t renderY, size_t cZoom);

void screenToMap(int sX, int sY, size_t renderX, size_t renderY, size_t cZoom, double *mapX, double *mapY, size_t maxX);

int doAll(struct graph *G, size_t *path, size_t pathLength);

#endif
