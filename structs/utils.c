#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include "utils.h"

struct graph* buildGraph(const char *filepath){
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

    printf("number of lines of the file is : %zu\n", numberOfLines);

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
    int traffic;

    printf("just before loop all good\n");

    while (!(feof(file))) {
        tmp = fgetc(file);
        if(tmp == '/') {
            fscanf(file, "/%zu,%zu,%zu,%zu,", &interIndex, &x, &y, &nbLinks);
            printf("/%ln,%ln,%ln,%ln,\n", &interIndex, &x, &y, &nbLinks);
            printf("interIndex = %zu\n", interIndex);
            initInter(*G, interIndex, nbLinks);
            setInter(*G, interIndex, x, y, nbLinks);
            /*fscanf(file, "%zu", &interIndex);
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
            */
            size_t i = 0;
            while (i < nbLinks) {
                printf("i = %zu\n", i);
                if ((i + 1) == nbLinks)
                    fscanf(file, "%zu-%i-%zu*\n", &end, &traffic, &maxSpeed);
                else
                    fscanf(file, "%zu-%i-%zu_", &end, &traffic, &maxSpeed);
                /*fscanf(file, "%zu", &end);
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
                */
                i++;
                /*
                G.inters[interIndex].links[i].end = end;
                G.inters[interIndex].links[i].traffic = traffic;
                G.inters[interIndex].links[i].maxSpeed = maxSpeed; */
                setLink(G->inters[interIndex], i, end, 0, traffic, maxSpeed);
            }
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
    return G;
    fclose(file);
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
