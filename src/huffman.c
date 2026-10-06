#include <stdio.h>
#include <stdlib.h>

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

// Compress the input file using the generated Huffman codes
int compressFile(
    const char *inputFilename,
    const char *outputFilename,
    unsigned long long frequency[256],
    char *codes[256])
{

    FILE *input = fopen(inputFilename, "rb");

    if (input == NULL)
    {
        printf("Could not open input file\n");
        return 0;
    }

    FILE *output = fopen(outputFilename, "wb");

    if (output == NULL)
    {
        printf("Could not create output file\n");
        fclose(input);
        return 0;
    }

    // -----------------------------
    // Calculate original file size
    // -----------------------------

    fseek(input, 0, SEEK_END);

    unsigned long long originalSize =
        ftell(input);

    fseek(input, 0, SEEK_SET);

    // -----------------------------
    // Create header
    // -----------------------------

    HuffmanHeader header;

    initializeHeader(
        &header,
        originalSize,
        frequency);

    // -----------------------------
    // Write header
    // -----------------------------

    if (!writeHeader(
            output,
            &header))
    {

        printf("Could not write header\n");

        fclose(input);
        fclose(output);

        return 0;
    }

    // -----------------------------
    // Initialize bit writer
    // -----------------------------

    BitWriter writer;

    initBitWriter(
        &writer,
        output);

    // -----------------------------
    // Encode input file
    // -----------------------------

    unsigned char byte;

    while (fread(&byte, 1, 1, input) == 1)
    {

        writeBits(
            &writer,
            codes[byte]);
    }

    // Write remaining bits
    flushBitWriter(&writer);

    fclose(input);
    fclose(output);

    return 1;
}