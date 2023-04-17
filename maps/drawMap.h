#ifndef DRAWMAP_H
#define DRAWMAP_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../structs/graph.h"
#include "../algo/dijkstra.h"

void draw_vertex(SDL_Renderer *renderer, int x, int y, int radius);

void intersection_point(double x1, double y1, double x2, double y2, double* intersect_x, double* intersect_y);

void draw_map(SDL_Renderer *renderer, struct graph *G, size_t *path, size_t pathLength, size_t maxX, size_t maxY, size_t renderX, size_t renderY, size_t cZoom);

int doAll(struct graph *G, size_t *path, size_t pathLength);

#endif
