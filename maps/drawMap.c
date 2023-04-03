#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "drawMap.h"
#include "../structs/graph.h"
#include "../algo/dijkstra.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define VERTEX_RADIUS 5

void draw_vertex(SDL_Renderer *renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}


void draw_map(SDL_Renderer *renderer, struct graph *G) {
    // Set the draw color for vertices
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // Draw the vertices
    for (size_t i = 0; i < G->order; i++) {
        int x = G->inters[i].x;
        int y = G->inters[i].y;
        draw_vertex(renderer, x, y, VERTEX_RADIUS);
    }

    // Set the draw color for edges
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the edges
    for (size_t i = 0; i < G->order; i++) {
        for (size_t j = 0; j < G->inters[i].nblinks; j++) {
            size_t end = G->inters[i].links[j].end;
            SDL_RenderDrawLine(renderer, G->inters[i].x, G->inters[i].y, G->inters[end].x, G->inters[end].y);
        }
    }
}


int doAll(size_t map) {
    char *filepath = calloc(50, sizeof(char));
    sprintf(filepath, "maps/%zu.txt", map);
    struct graph *G = buildGraph(filepath); // Create your graph here
    free(filepath);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Map Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the map
        draw_map(renderer, G);

        // Present the rendered scene
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    freeGraph(G);

    return 0;
}


