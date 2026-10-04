#include "bit_io.h"

// intialize the BitWriter
void initBitWriter(
    BitWriter *writer,
    FILE *file
) {

    writer->file = file;

    writer->buffer = 0;

    writer->bitCount = 0;
}

// write a single bit to the BitWriter
void writeBits(
    BitWriter *writer,
    const char *bits
) {

    for (int i = 0; bits[i] != '\0'; i++) {

        writeBit(
            writer,
            bits[i] == '1'
        );
    }
}

// flush the BitWriter
void flushBitWriter(
    BitWriter *writer
) {

    if (writer->bitCount > 0) {

        writer->buffer =
            writer->buffer
            << (8 - writer->bitCount);

        fputc(
            writer->buffer,
            writer->file
        );

        writer->buffer = 0;

        writer->bitCount = 0;
    }
}