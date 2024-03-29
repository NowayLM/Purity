#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "graph.h"
#include "../algo/dijkstra.h"

struct graph *buildGraph(const char *filepath) {
    FILE *file;
    char tmp;
    file = fopen(filepath, "r");

    if (file == NULL) {
        err(3, "file can't be opened");
    }
    size_t numberOfLines = 0;
    while (tmp != EOF) {
        if (tmp == '/') {
            numberOfLines += 1;
        }
        tmp = getc(file);
    }

    int returnCode = fseek(file, 0, SEEK_SET);
    if (returnCode != 0) {
        err(3, "impossible to go back to file start");
    }

    struct graph *G = initGraph(numberOfLines);

    size_t interIndex;
    size_t x;
    size_t y;
    size_t nbLinks;
    size_t end;
    size_t maxSpeed;
    size_t distance;
    int traffic;

    // Read the intersections first
    while (!(feof(file))) {
        tmp = fgetc(file);
        if(tmp == '/') {
            fscanf(file, "%zu,%zu,%zu,%zu,", &interIndex, &x, &y, &nbLinks);
            initInter(*G, interIndex, nbLinks);
            setInter(*G, interIndex, x, y, nbLinks);
        }
    }

    // Reset file pointer to the beginning
    fseek(file, 0, SEEK_SET);

    // Read the links and set distances
    while (!(feof(file))) {
        tmp = fgetc(file);
        if (tmp == '/') {
            fscanf(file, "%zu,%zu,%zu,%zu,", &interIndex, &x, &y, &nbLinks);
            for (size_t i = 0; i < nbLinks; i++) {
                if ((i + 1) == nbLinks)
                    fscanf(file, "%zu-%i-%zu*\n", &end, &traffic, &maxSpeed);
                else
                    fscanf(file, "%zu-%i-%zu_", &end, &traffic, &maxSpeed);

                distance = euclidean_distance((double)G->inters[interIndex].x, (double)G->inters[interIndex].y, (double)G->inters[end].x, (double)G->inters[end].y);
                //printf("Link between %zu and %zu with length: %zu\n", interIndex, end, distance);
                setLink(G->inters[interIndex], i, end, distance, traffic, maxSpeed);
            }
        }
    }

    fclose(file);
    return G;
}






void initInter(struct graph G, size_t nbInter, size_t nblinks)
{
    /* initInter initialise une nouvelle intersection soit un nouveau sommet que
    l'on peut ensuite ajouter a un graph*/
    G.inters[nbInter].links = calloc(nblinks, sizeof(struct link));
}

void setLink(struct inter inter, size_t index, size_t end, size_t length, int traffic, size_t maxSpeed) {
    /* setLink remplis les attributs du lien inter.links[index]*/
    struct link *newLink = inter.links + index;//calloc(1, sizeof(struct link));
    newLink->end = end;
    newLink->length = length;
    newLink->maxSpeed = maxSpeed;
    newLink->traffic = traffic;
}

void setInter(struct graph G, size_t index, size_t x, size_t y, size_t nblinks) {
    /* setInter ajoute l'intersection inter dans le graph G à l'index index*/
    G.inters[index].number = index;
    G.inters[index].x = x;
    G.inters[index].y = y;
    G.inters[index].nblinks = nblinks;
}

struct graph *initGraph(size_t order) {
    /* alloue la mémoire nécéssaire à un Graph G contenant order sommets et le retourne*/
    struct graph *newGraph = calloc(1, sizeof(struct graph));
    newGraph->order = order;
    newGraph->inters = calloc(order, sizeof(struct inter));
    return newGraph;
}

void freeGraph(struct graph *G) {
    if (G == NULL) return;
    for (size_t i = 0; i < G->order; i++)
        freeInter(G->inters + i);
    free(G->inters);
    free(G);
}

void freeInter(struct inter *inter) {
    /* libère la mémoire de l'intersection inter et de tous ses attributs*/
    if (inter == NULL) return;

    // loop through all links to free names
    for (size_t i = 0; i < inter->nblinks; i++) {
        char *name = inter->links[i].name;
        if (name != NULL)
            free(name);
    }

    // free all links at once
    free(inter->links);
}
