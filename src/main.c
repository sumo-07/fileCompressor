#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

int main() {

    unsigned long frequency[256] = {0};

    frequency['A'] = 5;
    frequency['B'] = 2;
    frequency['C'] = 1;
    frequency['D'] = 1;

    HuffmanNode *root =
        buildHuffmanTree(frequency);

    if (root == NULL) {
        printf("Failed to build tree\n");
        return 1;
    }

    char *codes[256] = {NULL};

    char code[256];

    generateCodes(
        root,
        code,
        0,
        codes
    );

    printf("\nGenerated codes:\n");

    for (int i = 0; i < 256; i++) {

        if (codes[i] != NULL) {

            printf(
                "%c -> %s\n",
                (unsigned char)i,
                codes[i]
            );
        }
    }

    // Free codes
    for (int i = 0; i < 256; i++) {
        free(codes[i]);
    }

    freeTree(root);

    return 0;
}