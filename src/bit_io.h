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

int writeBit(
    BitWriter *writer,
    int bit
);

int writeBits(
    BitWriter *writer,
    const char *bits
);

int flushBitWriter(
    BitWriter *writer
);


typedef struct {
    FILE *file;
    unsigned char buffer;
    int bitCount;
} BitReader;

void initBitReader(BitReader *reader, FILE *file);
int readBit(BitReader *reader);


#endif