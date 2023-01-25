#include <stdio.h>
#include <string.h>
#include "../structs/utils.h"

int main(){
    struct graph G = initGraph(5000000);
    struct inter tmp;
    for(size_t i = 0; i < G.order; i++) {
        tmp = initInter(i, i, i, 4);
        for(size_t j = 0; j < tmp.nblinks; j++) {
            setLink(tmp,j, j, 10, 1, 80);
        }
        setInter(G, i, tmp);
    }
}