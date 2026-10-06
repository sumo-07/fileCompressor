#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct HuffmanNode
{

    unsigned char data;
    unsigned long long frequency;

    struct HuffmanNode *left;
    struct HuffmanNode *right;

} HuffmanNode;

HuffmanNode *createNode(
    unsigned char data,
    unsigned long long frequency);

int createFrequencyNodes(
    unsigned long long frequency[256],
    HuffmanNode *nodes[256]);

HuffmanNode *buildHuffmanTree(
    unsigned long long frequency[256]);

void printTree(
    HuffmanNode *root,
    int depth);


void freeTree(HuffmanNode *root);

void generateCodes(
    HuffmanNode *root,
    char *code,
    int depth,
    char *codes[256]
);


int compressFile(
    const char *inputFilename,
    const char *outputFilename,
    unsigned long long frequency[256],
    char *codes[256]
);




#endif