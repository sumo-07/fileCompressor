#include <stdio.h>

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
        printf("Failed to build Huffman tree\n");
        return 1;
    }

    printf(
        "Root frequency: %lu\n",
        root->frequency
    );

    printTree(root, 0);
    freeTree(root);

    return 0;
}