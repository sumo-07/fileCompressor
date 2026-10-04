#ifndef BIT_IO_H
#define BIT_IO_H

#include <stdio.h>

typedef struct {

    FILE *file;

    unsigned char buffer;

    int bitCount;

} BitWriter;

void initBitWriter(
    BitWriter *writer,
    FILE *file
);

void writeBit(
    BitWriter *writer,
    int bit
);

void writeBits(
    BitWriter *writer,
    const char *bits
);

void flushBitWriter(
    BitWriter *writer
);

#endif