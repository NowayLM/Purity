#ifndef FINALMAP_H
#define FINALMAP_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../structs/graph.h"
#include "../algo/dijkstra.h"
#include "drawMap.h"

void draw(SDL_Renderer *renderer, struct graph *G, size_t maxX, size_t maxY, size_t renderX, size_t renderY, size_t cZoom);

int windowHandle(struct Graph *G);


#endif