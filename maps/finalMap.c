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
#include <SDL2/SDL_ttf.h>


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


void draw_text(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect textRect;

    // Create surface from font
    surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        fprintf(stderr, "Failed to create text surface: %s\n", TTF_GetError());
        return;
    }

    // Create texture from surface
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    // Determine the positions of the text
    textRect.x = x;
    textRect.y = y;
    textRect.w = surface->w;
    textRect.h = surface->h;

    // Copy the texture with the text to the renderer
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Clean up
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


int windowHandle(struct graph *G) {
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

    if (TTF_Init() < 0) {
        fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("maps/OpenSans-Semibold.ttf", 30);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
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
    size_t pl1;
    size_t pl2;
    size_t pl3;
    size_t *path1;
    size_t *path2;
    size_t *path3;
    bool drawNumbers = true;
    bool drawCosts = true;
    bool pathsC = false;
    bool drawPath = false;
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
                    case SDLK_c:
                        if (selectedPoint == true && selectedPoint2 == true) {
                            pathsC = true;
                        }
                        break;
                    case SDLK_z:
                        if (drawNumbers == true)
                            drawNumbers = false;
                        else
                            drawNumbers = true;
                        break;
                    case SDLK_x:
                        if (drawCosts == true)
                            drawCosts = false;
                        else
                            drawCosts = true;
                        break;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (selectedPoint == false) {
                        SDL_GetMouseState(&mX, &mY);
                        screenToMap(mX, mY, renderX, renderY, cZoom, &mapMX, &mapMY, maxX);
                        for (size_t j = 0; j < G->order; j++) {
                            //printf("j = %zu\n", j);
                            if (fabs((double) G->inters[j].x - mapMX) <= 4 && fabs((double) G->inters[j].y - mapMY) <= 4) {
                                selectedInter = j;
                                //printf("fabs G[j].x - mapMX = %f\n", fabs((double) G->inters[j].x - mapMX));
                                //printf("selectedInter = %zu", selectedInter);
                                selectedPoint = true;
                                break;
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

        long newRadiusL = 70/(log(G->order * 100));
        int newRadius = (int) newRadiusL;

        // Draw the map
        if (pathsC == true) {
            //printf("s = %i || e = %i\n", selectedPoint, selectedPoint2);
            path1 = dijkstra(G, selectedInter, selectedInter2, &pl1, 1);
            path2 = dijkstra(G, selectedInter, selectedInter2, &pl2, 3);
            path3 = dijkstra(G, selectedInter, selectedInter2, &pl3, 4);
            drawPath = true;
            pathsC = false;
        }


        
        draw(renderer, G, maxX, maxY, renderX, renderY, cZoom);
        if (selectedPoint == true) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //sX = compute_pos(sX, )
            int FX = compute_pos(selectedInter, 0, renderX, renderY, maxX, cZoom, 1, G);
            int FY = compute_pos(selectedInter, 1, renderX, renderY, maxX, cZoom, 1, G);
            draw_vertex(renderer, FX, FY, newRadius);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        if (selectedPoint2 == true) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //sX = compute_pos(sX, )
            int GX = compute_pos(selectedInter2, 0, renderX, renderY, maxX, cZoom, 1, G);
            int GY = compute_pos(selectedInter2, 1, renderX, renderY, maxX, cZoom, 1, G);
            draw_vertex(renderer, GX, GY, newRadius);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        if (drawPath == true) {
            SDL_SetRenderDrawColor(renderer, 237, 208, 102, 255);
            draw_vertex(renderer, compute_pos(path1[pl1 - 1], 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(path1[pl1 - 1], 1, renderX, renderY, maxX, cZoom, 1, G), newRadius);
            for (size_t i = 0; i < pl1 - 1; i++) {
                size_t start = path1[i];
                draw_vertex(renderer, compute_pos(start, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(start, 1, renderX, renderY, maxX, cZoom, 1, G), newRadius);
                size_t end = path1[i + 1];
                SDL_RenderDrawLine(renderer, compute_pos(start, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(start, 1, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 1, renderX, renderY, maxX, cZoom, 1, G));
            }

            SDL_SetRenderDrawColor(renderer, 98, 145, 81, 255);
            draw_vertex(renderer, compute_pos(path2[pl2 - 1], 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(path2[pl2 - 1], 1, renderX, renderY, maxX, cZoom, 1, G), newRadius);
            for (size_t i = 0; i < pl2 - 1; i++) {
                size_t start = path2[i];
                draw_vertex(renderer, compute_pos(start, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(start, 1, renderX, renderY, maxX, cZoom, 1, G), newRadius);
                size_t end = path2[i + 1];
                SDL_RenderDrawLine(renderer, compute_pos(start, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(start, 1, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 1, renderX, renderY, maxX, cZoom, 1, G));
            }


            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            draw_vertex(renderer, compute_pos(path3[pl3 - 1], 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(path3[pl3 - 1], 1, renderX, renderY, maxX, cZoom, 1, G), newRadius);
            for (size_t i = 0; i < pl3 - 1; i++) {
                size_t start = path3[i];
                draw_vertex(renderer, compute_pos(start, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(start, 1, renderX, renderY, maxX, cZoom, 1, G), newRadius);
                size_t end = path3[i + 1];
                SDL_RenderDrawLine(renderer, compute_pos(start, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(start, 1, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 0, renderX, renderY, maxX, cZoom, 1, G), compute_pos(end, 1, renderX, renderY, maxX, cZoom, 1, G));
            }

        }
        if (drawNumbers == true) {
            SDL_Color color = {255, 255, 255, 255};
            for(size_t i = 0; i < G->order; i++) {
                int rx = compute_pos(i, 0, renderX, renderY, maxX, cZoom, 1, G);
                int ry = compute_pos(i, 1, renderX, renderY, maxX, cZoom, 1, G);
                char str[20];

                sprintf(str, "%zu", i);
                const char* const_str = str;
                draw_text(renderer, const_str, rx, ry, font, color);
            }
        }

        if (drawCosts == true) {
            SDL_Color color = {255, 255, 255, 255};
            for(size_t i = 0; i < G->order; i++) {
                for(size_t j = 0; j < G->inters[i].nblinks; j++) {
                    size_t end = G->inters[i].links[j].end;
                    int rx1 = (G->inters[j].x + G->inters[end].x) / 2;
                    int ry1 = (G->inters[j].y + G->inters[end].y) / 2;
                

                    int rx = compute_pos(rx1, 0, renderX, renderY, maxX, cZoom, 0, G);
                    int ry = compute_pos(ry1, 1, renderX, renderY, maxX, cZoom, 0, G);

                    printf("start = %zu, end = %zu, rx = %i, ry = %i\n", j, end, rx, ry);

                    char str[20];

                    size_t cost1 = cost(j, end, G, 1);
                    //printf("cost1 = %zu\n", cost1);
                    cost1 /= 100000000000;

                    sprintf(str, "%zu", cost1);
                    const char* const_str = str;
                    draw_text(renderer, const_str, rx, ry, font, color);
                }
                
            }
        }
        

        // Present the rendered scene
        SDL_RenderPresent(renderer);

        Uint32 frame_time = SDL_GetTicks() - start_time;
        if (frame_time < 1000 / 60)
            SDL_Delay(1000 / 60 - frame_time);
    }

    // Clean up
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    if (path1 != NULL) free(path1);
    if (path2 != NULL) free(path2);
    if (path3 != NULL) free(path3);

    return 0;
}
