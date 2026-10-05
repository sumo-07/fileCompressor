#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

int main() {

    // -----------------------------
    // Step 1: Read input file
    // -----------------------------

    FILE *file = fopen("../tests/test.txt", "rb");

    if (file == NULL) {
        printf("Could not open input file\n");
        return 1;
    }

    // -----------------------------
    // Step 2: Frequency analysis
    // -----------------------------

    unsigned long frequency[256] = {0};

    unsigned char byte;

    while (fread(&byte, 1, 1, file) == 1) {
        frequency[byte]++;
    }

    fclose(file);

    // -----------------------------
    // Step 3: Build Huffman tree
    // -----------------------------

    HuffmanNode *root =
        buildHuffmanTree(frequency);

    if (root == NULL) {
        printf("Could not build Huffman tree\n");
        return 1;
    }

    // -----------------------------
    // Step 4: Generate codes
    // -----------------------------

    char *codes[256] = {NULL};

    char code[256];

    generateCodes(
        root,
        code,
        0,
        codes
    );

    printf("\nHuffman Codes:\n");

    for (int i = 0; i < 256; i++) {

        if (codes[i] != NULL) {

            printf(
                "%d ('%c') -> %s\n",
                i,
                (unsigned char)i,
                codes[i]
            );
        }
    }

    // -----------------------------
    // Step 5: Compress
    // -----------------------------

    if (compressFile(
            "../tests/test.txt",
            "output.bin",
            codes)) {

        printf("\nCompression successful!\n");

    } else {

        printf("\nCompression failed!\n");
    }

    // -----------------------------
    // Cleanup
    // -----------------------------

    for (int i = 0; i < 256; i++) {
        free(codes[i]);
    }

    freeTree(root);

    return 0;
}