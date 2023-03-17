#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "../structs/graph.h"
#include "../structs/queue.h"
#include "dijkstra.h"
#include <stddef.h>
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


size_t* dijkstra(struct graph *g, size_t source, size_t dest, size_t *path_length) {
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
}
