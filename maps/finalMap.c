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


int main