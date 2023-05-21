#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int main() {
    FILE *file;

    file = fopen("6.txt", "w");
    if (file == NULL)
        return 1;
    size_t rootSize = 50;
    for(size_t y = 0; y < rootSize; y++) {
        for(size_t x = 0; x < rootSize; x++) {
            if (y == 0) {
                if (x == 0) {
                    fprintf(file, "/0,0,0,2,1-1-130_%zu-1-130*\n", rootSize);
                }
                else {
                    if (x == rootSize - 1){
                        fprintf(file, "/%zu,%zu,%u,2,%zu-1-130_%zu-1-130*\n", x, x * 10, 0, rootSize - 2, rootSize * 2 - 1);
                    }
                    else fprintf(file, "/%zu,%zu,%u,3,%zu-1-130_%zu-1-130_%zu-1-130*\n",x, x * 10, 0, x - 1, x + 1, x + rootSize);
                }
            }
            else {
                if (y == rootSize - 1){
                    if (x == 0) {
                    fprintf(file, "/%zu,%zu,%zu,2,%zu-1-130_%zu-1-130*\n", y * rootSize, x * 10, y * 10, (y - 1) * rootSize, y * rootSize + 1);
                    }
                    else {
                    if (x == rootSize - 1){
                        fprintf(file, "/%zu,%zu,%zu,2,%zu-1-130_%zu-1-130*\n", rootSize * rootSize - 1, x * 10, y * 10, (rootSize * rootSize) - 2, y * rootSize - 1);
                    }
                    else fprintf(file, "/%zu,%zu,%zu,3,%zu-1-130_%zu-1-130_%zu-1-130*\n",y * rootSize + x, x * 10, y * 10, y * rootSize + x - 1, y * rootSize + x + 1, y * rootSize + x - rootSize);
                    }
                }
                else {
                    if (x == 0) {
                        fprintf(file, "/%zu,%zu,%zu,3,%zu-1-130_%zu-1-130_%zu-1-130*\n", y * rootSize, x * 10, y * 10, (y - 1) * rootSize, y * rootSize + 1, (y - 1) * rootSize);
                    }
                    else {
                        if (x == rootSize - 1){
                            fprintf(file, "/%zu,%zu,%zu,3,%zu-1-130_%zu-1-130_%zu-1-130*\n", y * rootSize + x, x * 10, y * 10, (y - 1) * rootSize + x, y * rootSize + x - 1, (y - 1) * rootSize + x);
                        }
                    else fprintf(file, "/%zu,%zu,%zu,4,%zu-1-130_%zu-1-130_%zu-1-130_%zu-1-130*\n",y * rootSize + x, x * 10, y * 10, (y * rootSize + x) - 1, (y * rootSize + x) + 1, ((y - 1) * rootSize + x), ((y + 1) * rootSize + x));
                    }
                }
            }
        }
    }
    fclose(file);
    return 0;
}