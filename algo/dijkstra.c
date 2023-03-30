#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "../structs/graph.h"
#include "../structs/queue.h"
#include "dijkstra.h"
#include <stddef.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>


size_t cost (size_t x, size_t y, struct graph *G) {
    if (x == y) return 0;
    for (size_t i = 0; i < G->inters[x].nblinks; i++) {
        if (G->inters[x].links[i].end == y) {
            size_t length = G->inters[x].links[i].length;
            size_t traffic = 1 + G->inters[x].links[i].traffic;
            size_t maxSpeed = 1 + G->inters[x].links[i].maxSpeed;
            size_t result = ((length)*(10*(traffic)))/(maxSpeed);
            return (result);
        }
    }
    return SIZE_MAX;
}

size_t compute_path_length (size_t path_length, size_t *path, struct graph *G) {
    size_t total_length = 0;
    for (size_t i = 0; i < path_length - 1; i++) {
        size_t u = path[i];
        size_t v = path[i + 1];
        for (size_t j = 0; j < G->inters[u].nblinks; j++) {
            if (G->inters[u].links[j].end == v) {
                total_length += G->inters[u].links[j].length;
                //printf("distance between %zu and %zu is %zu\n", u, v, G->inters[u].links[j].length);
                break;
            }
        }
    }
    return total_length;
}


size_t euclidean_distance(double x1, double y1, double x2, double y2) {
    return (size_t)round(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

size_t *dijkstra(struct graph *g, size_t start, size_t end, size_t *path_length){
    size_t *dist = malloc(g->order * sizeof(size_t));
    size_t *prev = malloc(g->order * sizeof(size_t));
    bool *visited = calloc(g->order, sizeof(bool));

    for (size_t i = 0; i < g->order; i++) {
        dist[i] = (i == start) ? 0 : SIZE_MAX;
        prev[i] = SIZE_MAX;
    }

    for (size_t i = 0; i < g->order; i++) {
        size_t u = SIZE_MAX;

        for (size_t j = 0; j < g->order; j++) {
            if (!visited[j] && (u == SIZE_MAX || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == SIZE_MAX) {
            break;
        }

        visited[u] = true;

        if (u == end) {
                    break;
        }

        for (size_t j = 0; j < g->inters[u].nblinks; j++) {
            size_t v = g->inters[u].links[j].end;
            size_t alt = dist[u] + cost(u, v, g);

            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
            }
        }

    }

    // Build the path
    size_t path_length2 = 0;
    size_t current = end;

    while (current != SIZE_MAX) {
        path_length2++;
        current = prev[current];
    }

    size_t *path = malloc(path_length2 * sizeof(size_t));
    current = end;
    *path_length = path_length2;
    /*printf("distances : ");
    for (size_t i = 0; i < g->order; i++) {
        printf("%zu, ", dist[i]);
    }
    printf("\n"); */

    for (size_t i = 0; i < path_length2; i++) {
        path[path_length2 - 1 - i] = current;
        current = prev[current];
    }

    free(dist);
    free(prev);
    free(visited);

    return path;
}


size_t *groupeFunction(char *filepath, size_t start, size_t end, size_t *total_length) {
    /*POUR UTILISER CETTE FONCTION : filepath doit être une string (char *) contenant le chemin à prendre depuis le dossier du Makefile
                                     start et end sont des size_t devant être entre 0 et G.order - 1
                                     En C, on ne peut retourner qu'une variable par fonction, donc je je peux pas retourner ET le tableau
                                     qui contient le chemin ET sa longueur, donc on doit créer un size_t total_length avant d'appeler la
                                     fonction puis passer son adresse en paramètre (&total_length), la fonction remplira donc la variable
                                     total_length créée au préalable. Exemple : size_t total_length = 0;
                                                                                size_t *path = groupeFunction("maps/1.txt", 0, 1, &total_length);
                                    IL FAUT ABSOLUMENT FREE LA VARIABLE PATH QUAND VOUS AVEZ FINI, tout le reste est free automatiquement.
    */
    struct graph *G = buildGraph(filepath);
    if (start == end)
        errx(3, "start point can't be destination");
    if (start >= G->order || end >= G->order)
        errx(3, "start point and destination must be less than %zu.\n", G->order);
    printf("\n\nComputing path from %zu to %zu.\n\n", start, end);
    size_t path_length = 0;
    size_t *path = dijkstra(G, start, end, &path_length);
    *total_length = compute_path_length(path_length, path, G);
    freeGraph(G);
    free(filepath);
    return path;
}