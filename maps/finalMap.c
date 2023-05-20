#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <err.h>
#include <stddef.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "drawMap.h"
#include "finalMap.h"
#include "../structs/graph.h"
#include "../algo/dijkstra.h"
#include <sys/types.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

void draw(SDL_Renderer *renderer, struct graph *G, size_t maxX, size_t maxY, size_t renderX, size_t renderY, size_t cZoom) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    long newRadiusL = 70/(log(G->order * 100));
    int newRadius = (int) newRadiusL;

    if (maxX < maxY) maxX = maxY;

    // Draw the vertices
    for (size_t i = 0; i < G->order; i++) {
        int fx = compute_pos(i, 0, renderX, renderY, maxX, cZoom, 1, G);
        int fy = compute_pos(i, 1, renderX, renderY, maxX, cZoom, 1, G);
        draw_vertex(renderer, fx, fy, newRadius);
    }

    // Draw the edges
    for (size_t i = 0; i < G->order; i++) {
        for (size_t j = 0; j < G->inters[i].nblinks; j++) {
            size_t end = G->inters[i].links[j].end;
            SDL_RenderDrawLine(renderer, compute_pos(i, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(i, 1, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 1, renderX, renderY, maxX, cZoom, 1, G));
        }
    }
}


int windowHandle(struct Graph *G) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }
    int mX, mY;
    double mapMX, mapMY;
    size_t cZoom = 100;
    size_t renderX = 0;
    size_t renderY = 0;
    long RadiusL = 70/(log(G->order * 100));
    int Radius = (int) RadiusL;

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

    maxX += (maxX / 10) + 2;
    maxY += (maxY / 10) + 2;
    
    if (maxX < maxY) maxX = maxY;

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
    bool selectedPoint = false;
    bool running = true;
    double currentSpeedX = 0;
    double currentSpeedY = 0;
    double currentZoomSpeed = 0;
    size_t selectedInter;
    size_t selectedInter2;
    bool selectedPoint2 = false;
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
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (selectedPoint == false) {
                        SDL_GetMouseState(&mX, &mY);
                        screenToMap(mX, mY, renderX, renderY, cZoom, &mapMX, &mapMY, maxX);
                        for (size_t j = 0; j < G->order; j++) {
                            if (fabs((double) G->inters[j].x - mapMX) <= 4 && fabs((double) G->inters[j].y - mapMY) <= 4) {
                                selectedInter = j;
                                selectedPoint = true;
                            }
                        }
                    }
                    else {
                        if (selectedPoint2 == false) {
                            SDL_GetMouseState(&mX, &mY);
                            screenToMap(mX, mY, renderX, renderY, cZoom, &mapMX, &mapMY, maxX);
                            for (size_t j = 0; j < G->order; j++) {
                                if (fabs((double) G->inters[j].x - mapMX) <= 4 && fabs((double) G->inters[j].y - mapMY) <= 4) {
                                    selectedInter2 = j;
                                    selectedPoint2 = true;
                                }
                            }
                        }
                        else {
                            selectedPoint = false;
                            selectedPoint2 = false;
                        }
                    }
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
        if (selectedInter == 72) {
            running = false;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderClear(renderer);

        // Draw the map
        draw(renderer, G, maxX, maxY, renderX, renderY, cZoom);
        if (selectedPoint == true) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //sX = compute_pos(sX, )
            int FX = compute_pos(selectedInter, 0, renderX, renderY, maxX, cZoom, 1, G);
            int FY = compute_pos(selectedInter, 1, renderX, renderY, maxX, cZoom, 1, G);
            draw_vertex(renderer, FX, FY, Radius);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        if (selectedPoint2 == true) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //sX = compute_pos(sX, )
            int GX = compute_pos(selectedInter2, 0, renderX, renderY, maxX, cZoom, 1, G);
            int GY = compute_pos(selectedInter2, 1, renderX, renderY, maxX, cZoom, 1, G);
            draw_vertex(renderer, GX, GY, Radius);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }

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