#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "../structs/graph.h"
#include "../structs/queue.h"
#include "../algo/dijkstra.h"
#include "../algo/algo.h"
#include "drawMap.h"
#include "finalMap.h"


int main() {
    struct graph *G = buildGraph("maps/1.txt");
    windowHandle(G);
    return 0;
}