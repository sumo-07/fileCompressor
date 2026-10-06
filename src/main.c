#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

int main(int argc, char *argv[]) {

    const char *defaultInputFilename = "tests/test.txt";
    const char *fallbackInputFilename = "../tests/test.txt";
    const char *inputFilename = argc > 1 ? argv[1] : defaultInputFilename;
    const char *outputFilename = argc > 2 ? argv[2] : "output.huf";

    // ==================================
    // 1. Read input and count frequency
    // ==================================

    FILE *file = fopen(
        inputFilename,
        "rb"
    );

    if (file == NULL && argc <= 1) {
        file = fopen(
            fallbackInputFilename,
            "rb"
        );

        if (file != NULL) {
            inputFilename = fallbackInputFilename;
        }
    }

    if (file == NULL) {

        printf(
            "Could not open input file\n"
        );

        return 1;
    }

    unsigned long long frequency[256] = {0};

    unsigned char byte;

    while (
        fread(
            &byte,
            1,
            1,
            file
        ) == 1
    ) {

        frequency[byte]++;
    }

    fclose(file);


    // ==================================
    // 2. Build Huffman tree
    // ==================================

    HuffmanNode *root =
        buildHuffmanTree(frequency);

    if (root == NULL) {

        printf(
            "Could not build Huffman tree\n"
        );

        return 1;
    }


    // ==================================
    // 3. Generate Huffman codes
    // ==================================

    char *codes[256] = {NULL};

    char code[256];

    generateCodes(
        root,
        code,
        0,
        codes
    );


    // ==================================
    // 4. Compress
    // ==================================

    if (
        compressFile(
            inputFilename,
            outputFilename,
            frequency,
            codes
        )
    ) {

        printf(
            "\nCompression successful!\n"
        );

        printf(
            "Created: %s\n",
            outputFilename
        );

    } else {

        printf(
            "\nCompression failed!\n"
        );
    }


    // ==================================
    // 5. Cleanup
    // ==================================

    for (int i = 0; i < 256; i++) {

        free(codes[i]);
    }

    freeTree(root);

    return 0;
}