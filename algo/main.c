#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "../structs/graph.h"
#include "../structs/queue.h"
#include "dijkstra.h"
#include "algo.h"

int main(){
    /*printf("Please enter the filepath for the map.\n");
    char filepath[100];
    scanf("%s", filepath);*/
    printf("\n\n##########\n\nWelcome to Purity, the best guidance system.\n\n##########\n\n");
    printf("Choose which map you want to use (1-3)\n");
    size_t mapNumber;
    scanf("%zu", &mapNumber);
    char *filepath = calloc(50, sizeof(char));
    sprintf(filepath, "maps/%zu.txt", mapNumber);
    struct graph *G = buildGraph(filepath);
    printf("\nEnter the starting point and the destination, they must belong to [0 ; %zu] (format : start dest)\n", G->order - 1);
    size_t start;
    size_t end;
    scanf("%zu %zu", &start, &end);
    if (start == end)
        errx(3, "start point can't be destination");
    if (start >= G->order || end >= G->order)
        errx(3, "start point and dest must be less than %zu.\n", G->order);
    printf("\n\nComputing path from %zu to %zu.\n\n", start, end); 
    //size_t *path = ladder(G, start, end);
    size_t path_length = 0;
    size_t *path = dijkstra(G, start, end, &path_length);
    size_t i = 0;

    // Calculate the total length of the path
    size_t total_length = 0;
    for (i = 0; i < path_length - 1; i++) {
        size_t u = path[i];
        size_t v = path[i + 1];
        for (size_t j = 0; j < G->inters[u].nblinks; j++) {
            if (G->inters[u].links[j].end == v) {
                total_length += G->inters[u].links[j].length;
                printf("distance between %zu and %zu is %zu\n", u, v, G->inters[u].links[j].length);
                break;
            }
        }
    }

    printf("[");
    for (i = 0; i < path_length - 1; i++) {
        printf("%zu, ", path[i]);
    }
    printf("%zu]\n", path[path_length - 1]);
    printf("The distance is : %zu\n\n", total_length);
    freeAll(G, path);
    printf("Thank you for choosing Purity to guide you once again.\n");
    free(filepath);
    return 1;
}
