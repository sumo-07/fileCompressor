#ifndef HEAP_H
#define HEAP_H

#include "huffman.h"

#define MAX_HEAP_SIZE 256

typedef struct {

    HuffmanNode *nodes[MAX_HEAP_SIZE];

    int size;

} MinHeap;

void initHeap(MinHeap *heap);

void insertHeap(
    MinHeap *heap,
    HuffmanNode *node
);

HuffmanNode* extractMin(
    MinHeap *heap
);

#endif