// #include <stdio.h>
// #include <stdlib.h>

// #include "huffman.h"

// int main() {

//     FILE *file = fopen("tests/test.txt", "rb");

//     if (file == NULL) {
//         printf("Could not open file\n");
//         return 1;
//     }

//     // Step 3: Count frequencies
//     unsigned long frequency[256] = {0};

//     unsigned char byte;

//     while (fread(&byte, 1, 1, file) == 1) {
//         frequency[byte]++;
//     }

//     fclose(file);


//     // Step 5: Create Huffman nodes
//     HuffmanNode *nodes[256];

//     int nodeCount =
//         createFrequencyNodes(frequency, nodes);

//     if (nodeCount == -1) {
//         printf("Failed to create nodes\n");
//         return 1;
//     }


//     // Display nodes
//     printf("Number of nodes: %d\n\n", nodeCount);

//     for (int i = 0; i < nodeCount; i++) {

//         printf(
//             "Node %d: '%c' -> %lu\n",
//             i,
//             nodes[i]->data,
//             nodes[i]->frequency
//         );
//     }


//     // Cleanup
//     for (int i = 0; i < nodeCount; i++) {
//         free(nodes[i]);
//     }

//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"
#include "heap.h"

int main() {

    MinHeap heap;

    initHeap(&heap);

    HuffmanNode *a = createNode('A', 5);
    HuffmanNode *b = createNode('B', 2);
    HuffmanNode *c = createNode('C', 1);
    HuffmanNode *d = createNode('D', 7);

    insertHeap(&heap, a);
    insertHeap(&heap, b);
    insertHeap(&heap, c);
    insertHeap(&heap, d);

    printf("Extracting nodes:\n");

    HuffmanNode *node;

    while ((node = extractMin(&heap)) != NULL) {

        printf(
            "%c -> %lu\n",
            node->data,
            node->frequency
        );
    }

    free(a);
    free(b);
    free(c);
    free(d);

    return 0;
}