#ifndef DRAWMAP_H
#define DRAWMAP_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../structs/graph.h"
#include "../algo/dijkstra.h"

void draw_vertex(SDL_Renderer *renderer, int x, int y, int radius);

void draw_map(SDL_Renderer *renderer, struct graph *G);

int doAll(size_t map);

#endif