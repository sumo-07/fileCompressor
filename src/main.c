#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "file_format.h"
#include "bit_io.h"

// Decode the compressed data using the Huffman tree
int decodeFile(
    FILE *input,
    FILE *output,
    HuffmanNode *root,
    unsigned long long originalSize
) {
    if (originalSize == 0) {
        return 1;
    }

    if (root == NULL) {
        return 0;
    }

    // Special case: only one unique character
    if (root->left == NULL && root->right == NULL) {
        for (unsigned long long i = 0; i < originalSize; i++) {
            if (fputc(root->data, output) == EOF) {
                return 0;
            }
        }
        return 1;
    }

    BitReader reader;
    initBitReader(&reader, input);

    unsigned long long decodedBytes = 0;
    HuffmanNode *current = root;

    while (decodedBytes < originalSize) {
        int bit = readBit(&reader);

        if (bit == -1) {
            return 0;
        }

        if (bit == 0) {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current == NULL) {
            return 0;
        }

        // A leaf represents one decoded character
        if (current->left == NULL &&
            current->right == NULL) {

            if (fputc(current->data, output) == EOF) {
                return 0;
            }

            decodedBytes++;
            current = root;
        }
    }

    return 1;
}


// Decompress a file using Huffman coding
int decompressFile(
    const char *inputFilename,
    const char *outputFilename
) {
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

    HuffmanNode *root = buildHuffmanTree(header.frequency);

    if (root == NULL && header.originalSize > 0) {
        printf("Could not rebuild Huffman tree.\n");
        fclose(input);
        return 0;
    }

    FILE *output = fopen(outputFilename, "wb");

    if (output == NULL) {
        perror("Could not create output file");
        freeTree(root);
        fclose(input);
        return 0;
    }

    int success = decodeFile(
        input,
        output,
        root,
        header.originalSize
    );

    if (fclose(output) != 0) {
        success = 0;
    }

    freeTree(root);
    fclose(input);

    if (!success) {
        printf("Decompression failed.\n");
        return 0;
    }

    printf("Decompression successful!\n");
    printf("Created: %s\n", outputFilename);

    return 1;
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage:\n");
        printf("  %s compress <input> <output.huf>\n", argv[0]);
        printf("  %s decompress <input.huf> <output>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "compress") == 0) {
        if (compressFile(argv[2], argv[3])) {
            printf("Compression successful!\n");
            printf("Created: %s\n", argv[3]);
            return 0;
        }
        printf("Compression failed!\n");
        return 1;
    }

    if (strcmp(argv[1], "decompress") == 0) {
        return decompressFile(argv[2], argv[3]) ? 0 : 1;
    }

    printf("Unknown command: %s\n", argv[1]);
    printf("Use 'compress' or 'decompress'.\n");
    return 1;
}