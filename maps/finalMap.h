#ifndef FINALMAP_H
#define FINALMAP_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../structs/graph.h"
#include "../algo/dijkstra.h"
#include "drawMap.h"
#include <SDL2/SDL_ttf.h>

void draw(SDL_Renderer *renderer, struct graph *G, size_t maxX, size_t maxY, size_t renderX, size_t renderY, size_t cZoom);

void draw_text(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color);

int windowHandle(struct graph *G);


#endif