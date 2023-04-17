#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "drawMap.h"
#include "../structs/graph.h"
#include "../algo/dijkstra.h"
#include <sys/types.h>

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

void compute_intersection(double x1, double y1, double x2, double y2, double* intersect_x, double* intersect_y) {
    double slope = (y2 - y1) / (x2 - x1);
    double y_intercept = y1 - slope * x1;
    *intersect_x = 0;
    *intersect_y = y_intercept;
}


void draw_map(SDL_Renderer *renderer, struct graph *G, size_t *path, size_t pathLength, size_t maxX, size_t maxY, size_t renderX, size_t renderY, size_t cZoom) {
    // Set the draw color for vertices
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    long newRadiusL = 70/(log(G->order * 100));
    int newRadius = (int) newRadiusL;
    //printf("newRadius = %i\nnewRadiusL = %f\n", newRadius, log(G->order * 100);
    //maxX = maxX - minX;
    //maxY = maxY - minY;

    //if (diffX > diffY) diffX = diffY;
    maxX += (maxX / 10) + 2;
    maxY += (maxY / 10) + 2;


    if (maxX < maxY) maxX = maxY;

    int compute_pos(size_t i, int xOrY) {
    double res;
    if (xOrY == 0) {
        res = (G->inters[i].x - renderX) * WINDOW_WIDTH / (maxX * cZoom / 100);
    }
    else {
        res = (G->inters[i].y - renderY) * WINDOW_HEIGHT / (maxX * cZoom / 100);
    }

    if (res < 0 && xOrY == 0) {
        double x1 = G->inters[i].x;
        double y1 = G->inters[i].y;
        double x2 = 0; // This can be any value, as long as it's non-negative and not equal to x1
        double y2 = y1 + (y1 / x1) * (x2 - x1); // Choose y2 such that the line has the same slope as the original line
        double intersect_x, intersect_y;
        compute_intersection(x1, y1, x2, y2, &intersect_x, &intersect_y);
        res = intersect_y;
    }

    return (int) res;
}


    // Draw the vertices
    for (size_t i = 0; i < G->order; i++) {
        int fx = compute_pos(i, 0);
        int fy = compute_pos(i, 1);
        draw_vertex(renderer, fx, fy, newRadius);
    }

    // Set the draw color for edges
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw the edges
    for (size_t i = 0; i < G->order; i++) {
        for (size_t j = 0; j < G->inters[i].nblinks; j++) {
            size_t end = G->inters[i].links[j].end;
            SDL_RenderDrawLine(renderer, compute_pos(i, 0), compute_pos(i, 1), compute_pos(end, 0), compute_pos(end, 1));
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    draw_vertex(renderer, compute_pos(path[pathLength - 1], 0), compute_pos(path[pathLength - 1], 1), newRadius);
    for (size_t i = 0; i < pathLength - 1; i++) {
        size_t start = path[i];
        draw_vertex(renderer, compute_pos(start, 0), compute_pos(start, 1), newRadius);
        size_t end = path[i + 1];
        SDL_RenderDrawLine(renderer, compute_pos(start, 0), compute_pos(start, 1), compute_pos(end, 0), compute_pos(end, 1));
    }
}


int doAll(struct graph *G, size_t *path, size_t pathLength) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
    size_t cZoom = 100;
    size_t renderX = 0;
    size_t renderY = 0;

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
    double currentSpeedX = 0;
    double currentSpeedY = 0;
    double currentZoomSpeed = 0;
    while (running) {
        // Handle events
        Uint32 start_time = SDL_GetTicks();
        if (currentSpeedX > 0) currentSpeedX--;
        else if (currentSpeedX < 0) currentSpeedX++;
        if (currentSpeedY > 0) currentSpeedY--;
        else if (currentSpeedY < 0) currentSpeedY++;
        if (currentZoomSpeed > 0) currentZoomSpeed--;
        else if (currentZoomSpeed < 0) currentZoomSpeed++;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                double maxSpeed = 15;
                double zoomMaxSpeed = 10;
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (currentSpeedY > 2 - maxSpeed)
                            currentSpeedY -= 3;
                        break;
                    case SDLK_a:
                        if (currentSpeedX > 2 - maxSpeed)
                            currentSpeedX -= 3;
                        break;
                    case SDLK_s:
                        if (currentSpeedY < maxSpeed - 2)
                            currentSpeedY += 3;
                        break;
                    case SDLK_d:
                        if (currentSpeedX < maxSpeed - 2)
                            currentSpeedX += 3;
                        break;
                    case SDLK_q:
                        if (currentZoomSpeed > 1 - zoomMaxSpeed)
                            currentZoomSpeed -= 2;
                        break;
                    case SDLK_e:
                        if (currentZoomSpeed < zoomMaxSpeed - 1)
                            currentZoomSpeed += 2;
                        break;
                    case SDLK_t:
                        running = false;
                        break;
                }
            }
            if (currentZoomSpeed < 0) {
                if (cZoom >= 0 - currentZoomSpeed)
                    cZoom += currentZoomSpeed;
            }
            else cZoom += currentZoomSpeed;
            if (currentSpeedX < 0) {
                if (renderX >= 0 - currentSpeedX)
                    renderX += currentSpeedX;
            }
            else renderX += currentSpeedX;
            if (currentSpeedY < 0) {
                if (renderY >= 0 - currentSpeedY)
                    renderY += currentSpeedY;
            }
            else renderY += currentSpeedY;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderClear(renderer);

        // Draw the map
        draw_map(renderer, G, path, pathLength, maxX, maxY, renderX, renderY, cZoom);

        // Present the rendered scene
        SDL_RenderPresent(renderer);

        Uint32 frame_time = SDL_GetTicks() - start_time;
        if (frame_time < 1000 / 60)
            SDL_Delay(1000 / 60 - frame_time);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
