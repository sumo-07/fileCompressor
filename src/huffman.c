#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

HuffmanNode* createNode(unsigned char data, unsigned long frequency) {

    HuffmanNode *node = malloc(sizeof(HuffmanNode));

    if (node == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    node->data = data;
    node->frequency = frequency;

    node->left = NULL;
    node->right = NULL;

    return node;
}


int createFrequencyNodes(
    unsigned long frequency[256],
    HuffmanNode *nodes[256]
) {

    int count = 0;

    for (int i = 0; i < 256; i++) {

        if (frequency[i] > 0) {

            nodes[count] = createNode(
                (unsigned char)i,
                frequency[i]
            );

            if (nodes[count] == NULL) {
                return -1;
            }

            count++;
        }
    }

    return count;
}