#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include "drawMap.h"
#include "../structs/graph.h"
#include "../algo/dijkstra.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VERTEX_RADIUS 8

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


void draw_map(SDL_Renderer *renderer, struct graph *G, size_t *path, size_t pathLength) {
    // Set the draw color for vertices
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    size_t minX = G->inters[0].x;
    size_t maxX = G->inters[0].x;
    size_t minY = G->inters[0].y;
    size_t maxY = G->inters[0].y;

    for (size_t i = 1; i < G->order; i++) {
        if (G->inters[i].x < minX)
            minX = G->inters[i].x;
        if (G->inters[i].x > maxX)
            maxX = G->inters[i].x;
        if (G->inters[i].y < minY)
            minY = G->inters[i].y;
        if (G->inters[i].y > maxY)
            maxY = G->inters[i].y;
    }
    //size_t diffX = maxX - minX;
    //size_t diffY = maxY - minY;

    //if (diffX > diffY) diffX = diffY;
    maxX += (maxX / 10) + 2;
    maxY += (maxY / 10) + 2;

    if (maxX < maxY) maxX = maxY;

    // Draw the vertices
    for (size_t i = 0; i < G->order; i++) {
        size_t x = G->inters[i].x * WINDOW_WIDTH / maxX + 50;
        size_t y = G->inters[i].y * WINDOW_HEIGHT / maxX + 50;
        //printf("x %zu y %zu diffX %zu diffY %zu\n", x, y, diffX, diffY);
        int fx = (int) x;
        int fy = (int) y;
        draw_vertex(renderer, fx, fy, VERTEX_RADIUS);
    }

    // Set the draw color for edges
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw the edges
    for (size_t i = 0; i < G->order; i++) {
        for (size_t j = 0; j < G->inters[i].nblinks; j++) {
            size_t end = G->inters[i].links[j].end;
            SDL_RenderDrawLine(renderer, G->inters[i].x * WINDOW_WIDTH / maxX + 50, G->inters[i].y * WINDOW_HEIGHT / maxX + 50,
            G->inters[end].x * WINDOW_WIDTH / maxX + 50, G->inters[end].y * WINDOW_HEIGHT / maxX + 50);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    draw_vertex(renderer, G->inters[path[pathLength - 1]].x * WINDOW_WIDTH / maxX + 50, G->inters[path[pathLength - 1]].y * WINDOW_HEIGHT / maxX + 50, VERTEX_RADIUS);
    for (size_t i = 0; i < pathLength - 1; i++) {
        size_t start = path[i];
        draw_vertex(renderer, G->inters[start].x * WINDOW_WIDTH / maxX + 50, G->inters[start].y * WINDOW_HEIGHT / maxX + 50, VERTEX_RADIUS);
        size_t end = path[i + 1];
        SDL_RenderDrawLine(renderer, G->inters[start].x * WINDOW_WIDTH / maxX + 50, G->inters[start].y * WINDOW_HEIGHT / maxX + 50,
        G->inters[end].x * WINDOW_WIDTH / maxX + 50, G->inters[end].y * WINDOW_HEIGHT / maxX + 50);
    }
}


int doAll(struct graph *G, size_t *path, size_t pathLength) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Map Visualization", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH + 100, WINDOW_HEIGHT + 100, SDL_WINDOW_SHOWN);
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
        SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
        SDL_RenderClear(renderer);

        // Draw the map
        draw_map(renderer, G, path, pathLength);

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


