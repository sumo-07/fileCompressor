#include <stdio.h>

#include "heap.h"

// Initialize the heap
void initHeap(MinHeap *heap) {

    heap->size = 0;
}

//check which node has smaller frequency
static int isSmaller(
    HuffmanNode *a,
    HuffmanNode *b
) {

    return a->frequency < b->frequency;
}

// Insert a node into the heap
void insertHeap(
    MinHeap *heap,
    HuffmanNode *node
) {

    if (heap->size >= MAX_HEAP_SIZE) {
        printf("Heap is full\n");
        return;
    }

    int index = heap->size;

    heap->nodes[index] = node;

    heap->size++;

    // Move node upward
    while (index > 0) {

        int parent = (index - 1) / 2;

        if (!isSmaller(heap->nodes[index], heap->nodes[parent])) {

            break;
        }

        // Swap
        HuffmanNode *temp = heap->nodes[index];

        heap->nodes[index] = heap->nodes[parent];

        heap->nodes[parent] = temp;

        index = parent;
    }
}


// Extract the node with the minimum frequency
HuffmanNode* extractMin(
    MinHeap *heap
) {

    if (heap->size == 0) {
        return NULL;
    }

    HuffmanNode *minNode =
        heap->nodes[0];

    heap->size--;

    if (heap->size == 0) {
        return minNode;
    }

    heap->nodes[0] =
        heap->nodes[heap->size];

    int index = 0;

    // Heapify down
    while (1) {

        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int smallest = index;

        if (left < heap->size &&
            isSmaller(
                heap->nodes[left],
                heap->nodes[smallest])) {

            smallest = left;
        }

        if (right < heap->size &&
            isSmaller(
                heap->nodes[right],
                heap->nodes[smallest])) {

            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        HuffmanNode *temp =
            heap->nodes[index];

        heap->nodes[index] =
            heap->nodes[smallest];

        heap->nodes[smallest] = temp;

        index = smallest;
    }

    return minNode;
}