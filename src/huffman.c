#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>

#include "huffman.h"
#include "heap.h"
#include "bit_io.h"
#include "file_format.h"

HuffmanNode *createNode(unsigned char data, unsigned long long frequency)
{

    HuffmanNode *node = malloc(sizeof(HuffmanNode));

    if (node == NULL)
    {
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
    unsigned long long frequency[256],
    HuffmanNode *nodes[256])
{

    int count = 0;

    for (int i = 0; i < 256; i++)
    {

        if (frequency[i] > 0)
        {

            nodes[count] = createNode(
                (unsigned char)i,
                frequency[i]);

            if (nodes[count] == NULL)
            {
                for (int i = 0; i < count; i++)
                {
                    free(nodes[i]);
                }
                return -1;
            }

            count++;
        }
    }

    return count;
}

// Build the Huffman tree from the frequency array
HuffmanNode *buildHuffmanTree(
    unsigned long long frequency[256])
{

    MinHeap heap;

    initHeap(&heap);

    // Create nodes for every byte that exists
    HuffmanNode *nodes[256];

    int nodeCount =
        createFrequencyNodes(
            frequency,
            nodes);

    if (nodeCount <= 0)
    {
        return NULL;
    }

    // Put all nodes into the heap
    for (int i = 0; i < nodeCount; i++)
    {

        insertHeap(
            &heap,
            nodes[i]);
    }

    // Keep combining two smallest nodes
    while (heap.size > 1)
    {

        HuffmanNode *left =
            extractMin(&heap);

        HuffmanNode *right =
            extractMin(&heap);

        // Create internal node
        HuffmanNode *parent =
            createNode(
                0,
                left->frequency + right->frequency);

        if (parent == NULL)
        {
            freeTree(left);
            freeTree(right);
            while (heap.size > 0)
            {
                freeTree(extractMin(&heap));
            }
            return NULL;
        }

        parent->left = left;
        parent->right = right;

        insertHeap(
            &heap,
            parent);
    }

    // The last node is the root
    return extractMin(&heap);
}

// Print the Huffman tree (for debugging)
void printTree(
    HuffmanNode *root,
    int depth)
{

    if (root == NULL)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }

    // Leaf node
    if (root->left == NULL &&
        root->right == NULL)
    {

        printf(
            "'%c' : %llu\n",
            root->data,
            root->frequency);
    }
    else
    {

        printf(
            "* : %llu\n",
            root->frequency);
    }

    printTree(root->left, depth + 1);

    printTree(root->right, depth + 1);
}

// Free the Huffman tree
void freeTree(HuffmanNode *root)
{

    if (root == NULL)
    {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

// Generate Huffman codes for each byte
void generateCodes(
    HuffmanNode *root,
    char *code,
    int depth,
    char *codes[256])
{

    if (root == NULL)
    {
        return;
    }

    // We reached a leaf
    if (root->left == NULL &&
        root->right == NULL)
    {

        // Special case:
        // file contains only one unique byte
        if (depth == 0)
        {
            code[0] = '0';
            code[1] = '\0';
            depth = 1;
        }
        else
        {
            code[depth] = '\0';
        }

        codes[root->data] = malloc(
            (depth + 1) * sizeof(char));

        if (codes[root->data] == NULL)
        { // if memory allocation fails
            return;
        }

        for (int i = 0; i <= depth; i++)
        {
            codes[root->data][i] = code[i];
        }

        printf(
            "'%c' -> %s\n",
            root->data,
            codes[root->data]);

        return;
    }

    // Go left → add 0
    code[depth] = '0';

    generateCodes(
        root->left,
        code,
        depth + 1,
        codes);

    // Go right → add 1
    code[depth] = '1';

    generateCodes(
        root->right,
        code,
        depth + 1,
        codes);
}

// Encode the input file using the provided Huffman codes.
int writeCompressedData(
    const char *inputFilename,
    FILE *output,
    char *codes[256])
{
    FILE *input = fopen(inputFilename, "rb");

    if (input == NULL)
    {
        perror("Could not reopen input file");
        return 0;
    }

    BitWriter writer;
    initBitWriter(&writer, output);

    unsigned char byte;
    int success = 1;
    while (fread(&byte, 1, 1, input) == 1)
    {
        if (codes[byte] == NULL || !writeBits(&writer, codes[byte]))
        {
            success = 0;
            break;
        }
    }

    if (ferror(input) || !flushBitWriter(&writer))
    {
        success = 0;
    }

    if (fclose(input) != 0)
    {
        success = 0;
    }

    return success;
}

// Compress the input file using the complete Huffman workflow.
int compressFile(
    const char *inputFilename,
    const char *outputFilename)
{
    unsigned long long frequency[256] = {0};
    unsigned long long originalSize = 0;
    char *codes[256] = {0};
    HuffmanNode *root = NULL;

    struct stat inputStat;
    struct stat outputStat;
    if (stat(inputFilename, &inputStat) == 0 &&
        stat(outputFilename, &outputStat) == 0 &&
        inputStat.st_dev == outputStat.st_dev &&
        inputStat.st_ino == outputStat.st_ino)
    {
        fprintf(stderr, "Input and output files must be different.\n");
        return 0;
    }

    FILE *input = fopen(inputFilename, "rb");

    if (input == NULL)
    {
        perror("Could not open input file");
        return 0;
    }

    int byte;
    while ((byte = fgetc(input)) != EOF)
    {
        if (frequency[(unsigned char)byte] == ULLONG_MAX ||
            originalSize == ULLONG_MAX)
        {
            fprintf(stderr, "Input file is too large to represent.\n");
            fclose(input);
            return 0;
        }
        frequency[(unsigned char)byte]++;
        originalSize++;
    }

    int success = !ferror(input);
    if (fclose(input) != 0)
    {
        success = 0;
    }
    if (!success)
    {
        perror("Could not read input file");
        return 0;
    }

    if (originalSize > 0)
    {
        root = buildHuffmanTree(frequency);
        if (root == NULL)
        {
            fprintf(stderr, "Could not build Huffman tree.\n");
            return 0;
        }

        char code[256];
        generateCodes(root, code, 0, codes);
        for (int i = 0; i < 256; i++)
        {
            if (frequency[i] > 0 && codes[i] == NULL)
            {
                fprintf(stderr, "Could not generate Huffman codes.\n");
                success = 0;
                break;
            }
        }
    }

    if (success)
    {
        FILE *output = fopen(outputFilename, "wb");
        if (output == NULL)
        {
            perror("Could not create output file");
            success = 0;
        }
        else
        {
            HuffmanHeader header;
            initializeHeader(&header, originalSize, frequency);
            success = writeHeader(output, &header);
            if (success)
            {
                success = writeCompressedData(
                    inputFilename,
                    output,
                    codes);
            }
            if (fclose(output) != 0)
            {
                success = 0;
            }
            if (!success)
            {
                if (stat(outputFilename, &outputStat) == 0 &&
                    S_ISREG(outputStat.st_mode))
                {
                    remove(outputFilename);
                }
            }
        }
    }

    for (int i = 0; i < 256; i++)
    {
        free(codes[i]);
    }
    freeTree(root);
    return success;
}