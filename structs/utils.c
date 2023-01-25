#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "utils.h"

struct graph buildGraph(const char *filepath){
    /* buildGraph prends en paramètre la string filepath et retourne
    le Graph G remplis avec tous les sommets et leurs informations
    telles qu'indiquées dans le fichier filepath.
    */
    FILE* file;
    char tmp;
    file = fopen(filepath, "r");
 
    if (file == NULL) {
        err(3, "file can't be opened");
    }

    size_t numberOfLines = 0;

    while (tmp != EOF)
    {
        if (tmp == '/')
        {
            numberOfLines += 1;
        }
        tmp = getc(file);
    }

    int returnCode = fseek(file, 0, SEEK_SET);
    if (returnCode != 0) {
        err(3, "impossible to go back to file start");
    }

    struct graph G = initGraph(numberOfLines);

    size_t interIndex;
    size_t x;
    size_t y;
    size_t nbLinks;
    size_t end;
    size_t maxSpeed;
    int traffic;

    while (!(feof(file))) {
        tmp = fgetc(file);
        if(tmp == '/') {
            fscanf(file, "%zu", &interIndex);
            if (fgetc(file) != ',') {
                fclose(file);
                err(3, "error in file format : X");
            }
            fscanf(file, "%zu", &x);
            if (fgetc(file) != ',') {
                fclose(file);
                err(3, "error in file format : Y");
            }
            fscanf(file, "%zu", &y);
            if (fgetc(file) != ',') {
                fclose(file);
                err(3, "error in file format : nbLinks");
            }
            fscanf(file, "%zu", &nbLinks);
            if (fgetc(file) != ',') {
                fclose(file);
                err(3, "error in file format : link -> end");
            }
            size_t i = 0;
            while((tmp != '*') && (i < nbLinks)) {
                fscanf(file, "%zu", &end);
                if (fgetc(file) != '-') {
                    fclose(file);
                    err(3, "error in file format : link -> traffic");
                }
                fscanf(file, "%i", &traffic);
                if (fgetc(file) != '-') {
                    fclose(file);
                    err(3, "error in file format : link -> maxSpeed");
                }
                fscanf(file, "%zu", &maxSpeed);
                tmp = fgetc(file);
                if ((tmp != '_') || (tmp != '*')) {
                    fclose(file);
                    err(3, "error in file format : too many arguments for a link");
                }
                i++;
                G.inters[interIndex].links[i].end = end;
                G.inters[interIndex].links[i].traffic = traffic;
                G.inters[interIndex].links[i].maxSpeed = maxSpeed;
            }
            G.inters[interIndex].x = x;
            G.inters[interIndex].number = interIndex;
            G.inters[interIndex].y = y;
            G.inters[interIndex].nblinks = nbLinks;
        }
    }
    // for (size_t i = 0; i < G.order; i++) {
    //     for () {
    //         //remplissage des distances
    //         double dx = x;
    //             double dy = y;
    //             double Dlength = sqrt((dx * dx) + (y * y));
    //             size_t length = Dlength;
    //             G.inters[interIndex].links[i].length = length;
    //     }
    //}
    fclose(file);
}



struct inter initInter(size_t nbInter, size_t x, size_t y, size_t nblinks)
{
    /* initInter initialise une nouvelle intersection soit un nouveau sommet que
    l'on peut ensuite ajouter a un graph*/
    struct inter *newInter = calloc(1, sizeof(struct inter));
    newInter->number = nbInter;
    newInter->x = x;
    newInter->y = y;
    newInter->nblinks = nblinks;
    newInter->links = calloc(nblinks, sizeof(struct link));
    return *newInter;
}

void setLink(struct inter inter, size_t index, size_t end, size_t length, int traffic, size_t maxSpeed) {
    /* setLink remplis les attributs du lien inter.links[index]*/
    struct link *newLink = inter.links + index;//calloc(1, sizeof(struct link));
    newLink->end = end;
    newLink->length = length;
    newLink->maxSpeed = maxSpeed;
    newLink->traffic = traffic;
}

void setInter(struct graph G, size_t index, struct inter inter) {
    /* setInter ajoute l'intersection inter dans le graph G à l'index index*/
    G.inters[index] = inter;
}

struct graph initGraph(size_t order) {
    /* alloue la mémoire nécéssaire à un Graph G vide et le retourne*/
    struct graph *newGraph = calloc(1, sizeof(struct graph));
    newGraph->order = order;
    newGraph->inters = calloc(order, sizeof(struct inter));
    return *newGraph;
}

void freeGraph(struct graph *G);

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