#include <stdlib.h>

size_t strcut(char** dest, unsigned int start, unsigned int end, char * str) {
    size_t length = end - start;
    *dest = malloc(length);
    for (int i = start; i < end; i++) {
        (*dest)[i - start] = str[i];
    }
    return length;
}