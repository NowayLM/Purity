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


size_t find_min_distance(size_t *distances, bool *visited, size_t order)
{
    size_t min_distance = SIZE_MAX;
    size_t min_index = 0;

    for (size_t i = 0; i < order; i++) {
        if (!visited[i] && distances[i] <= min_distance) {
            min_distance = distances[i];
            min_index = i;
        }
    }

    return min_index;
}

size_t cost (size_t x, size_t y, struct graph *G) {
    if (x == y) return 0;
    for (size_t i = 0; i < G->inters[x].nblinks; i++) {
        if (G->inters[x].links[i].end == y) {
            size_t result = (G->inters[x].links[i].length);
            return (result);
        }
    }
    return SIZE_MAX;
}


size_t euclidean_distance(double x1, double y1, double x2, double y2) {
    return (size_t)round(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}


/*size_t* dijkstra(struct graph *g, size_t source, size_t dest, size_t *path_length) {
    // Create arrays for storing distances and visited flags
    size_t *distances = (size_t *) malloc(g->order * sizeof(size_t));
    bool *visited = (bool *) malloc(g->order * sizeof(bool));
    size_t *prev = (size_t *) malloc(g->order * sizeof(size_t));

    // Initialize distances, visited flags, and previous vertex array
    for (size_t i = 0; i < g->order; i++) {
        distances[i] = SIZE_MAX;
        visited[i] = false;
        prev[i] = SIZE_MAX;
    }

    // Set the distance of the source vertex to 0
    distances[source] = 0;

    for (size_t i = 0; i < g->order; i++)
        printf("%zu, ", distances[i]);
    printf("\n\n");

    // Loop through all vertices
    for (size_t i = 0; i < g->order - 1; i++) {
        // Find the vertex with the minimum distance
        size_t min_vertex = find_min_distance(distances, visited, g->order);

        // Mark the vertex as visited
        visited[min_vertex] = true;

        // Update the distances and previous vertices of the neighboring vertices
        for (size_t j = 0; j < g->inters[min_vertex].nblinks; j++) {
            size_t neighbor = g->inters[min_vertex].links[j].end;
            size_t distance = distances[min_vertex] + g->inters[min_vertex].links[j].length;

            if (!visited[neighbor] && distance < distances[neighbor]) {
                distances[neighbor] = distance;
                prev[neighbor] = min_vertex;
            }
        }
    }

    // Build the shortest path using the previous vertex array
    size_t *path = (size_t *) malloc(g->order * sizeof(size_t));
    size_t current = dest;
    size_t index = 0;

    while (current != SIZE_MAX) {
        path[index++] = current;
        current = prev[current];
    }

    // Reverse the order of the vertices in the path array
    for (size_t i = 0; i < index / 2; i++) {
        size_t temp = path[i];
        path[i] = path[index - i - 1];
        path[index - i - 1] = temp;
    }

    // Set the path length
    *path_length = distances[dest];
    printf("distances : ");
    for (size_t i = 0; i < g->order; i++) {
        printf("%zu, ", distances[i]);
    }
    printf("\nSIZE_MAX = %zu\n", SIZE_MAX);


    // Free the memory used by the arrays
    free(distances);
    free(visited);
    free(prev);

    // Return the path array
    return path;
}*/

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
    printf("distances : ");
    for (size_t i = 0; i < g->order; i++) {
        printf("%zu, ", dist[i]);
    }
    printf("\n");

    for (size_t i = 0; i < path_length2; i++) {
        path[path_length2 - 1 - i] = current;
        current = prev[current];
    }

    // Clean up
    free(dist);
    free(prev);
    free(visited);

    return path;
}
