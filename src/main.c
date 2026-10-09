#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "file_format.h"


// Decompress a file using Huffman coding
int decompressFile(const char *inputFilename) {
    FILE *input = fopen(inputFilename, "rb");

    if (input == NULL) {
        perror("Could not open compressed file");
        return 0;
    }

    HuffmanHeader header;

    if (!readHeader(input, &header)) {
        printf("Invalid or corrupted compressed file.\n");
        fclose(input);
        return 0;
    }

    printf("Original file size: %llu bytes\n",
           header.originalSize);

    HuffmanNode *root = buildHuffmanTree(header.frequency);

    if (root == NULL && header.originalSize > 0) {
        printf("Could not rebuild Huffman tree.\n");
        fclose(input);
        return 0;
    }

    printf("Huffman tree rebuilt successfully.\n");

    freeTree(root);
    fclose(input);

    return 1;
}


int main(int argc, char *argv[]) {

    if (argc >= 3 && strcmp(argv[1], "-d") == 0) {
        return decompressFile(argv[2]) ? 0 : 1;
    }

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