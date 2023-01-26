#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

struct graph {
    struct inter *inters;
    size_t order;
};

struct inter {
    size_t number;
    size_t x;
    size_t y;
    size_t nblinks;
    struct link *links;
};

struct link {
    char *name;
    size_t end;
    size_t length;
    int traffic;
    size_t maxSpeed;
};

struct graph *buildGraph(const char *filepath);

void initInter(struct graph G, size_t nbInter, size_t nblinks);

void setLink(struct inter inter, size_t index, size_t end, size_t length, int traffic, size_t maxSpeed);

void setInter(struct graph G, size_t index, size_t x, size_t y, size_t nblinks);

struct graph* initGraph(size_t order);

void freeGraph(struct graph *G);

void freeInter(struct inter *inter);

#endif