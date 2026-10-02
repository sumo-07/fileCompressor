#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

int main()
{

    FILE *file = fopen("../tests/test.txt", "rb");

    if (file == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    unsigned long frequency[256] = {0};

    unsigned char byte;

    while (fread(&byte, 1, 1, file) == 1)
    {
        frequency[byte]++;
    }

    fclose(file);

    printf("Frequency table:\n");

    for (int i = 0; i < 256; i++)
    {

        if (frequency[i] > 0)
        {

            printf(
                "%d -> %lu\n",
                i,
                frequency[i]);
        }
    }

    printf("----------------x--------x-----------\n");

    // build the Huffman tree
    HuffmanNode *root =
        buildHuffmanTree(frequency);

    if (root == NULL)
    {
        printf("Could not build Huffman tree\n");
        return 1;
    }

    // generate code table
    char *codes[256] = {NULL};

    char code[256];

    generateCodes(
        root,
        code,
        0,
        codes);

    printf("\nHuffman Codes:\n");

    for (int i = 0; i < 256; i++)
    {

        if (codes[i] != NULL)
        {

            printf(
                "%d ('%c') -> %s\n",
                i,
                (unsigned char)i,
                codes[i]);
        }
    }

    // encoding testing
    printf("\nEncoded bits:\n");

    encodeFile(
        "../tests/test.txt",
        codes);

    return 0;
}