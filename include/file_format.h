#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <stdio.h>

#define MAGIC "HUF1"

typedef struct {

    char magic[4];

    unsigned long long originalSize;

    unsigned long long frequency[256];

} HuffmanHeader;

void initializeHeader(
    HuffmanHeader *header,
    unsigned long long originalSize,
    unsigned long long frequency[256]
);

int writeHeader(
    FILE *file,
    HuffmanHeader *header
);

int readHeader(
    FILE *file,
    HuffmanHeader *header
);

#endif