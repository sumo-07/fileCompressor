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

// Write one bit to the BitWriter, emitting each completed byte.
void writeBit(
    BitWriter *writer,
    int bit
) {

    writer->buffer = (writer->buffer << 1) | (bit != 0);
    writer->bitCount++;

    if (writer->bitCount == 8) {
        fputc(writer->buffer, writer->file);
        writer->buffer = 0;
        writer->bitCount = 0;
    }
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

// BitReader functions
void initBitReader(BitReader *reader, FILE *file) {
    reader->file = file;
    reader->buffer = 0;
    reader->bitCount = 0;
}

int readBit(BitReader *reader) {
    // Load the next byte when no bits remain
    if (reader->bitCount == 0) {
        int byte = fgetc(reader->file);

        // End of file or read error
        if (byte == EOF) {
            return -1;
        }

        reader->buffer = (unsigned char)byte;
        reader->bitCount = 8;
    }

    // Extract the leftmost unread bit
    int bit = (reader->buffer >> 7) & 1;

    // Shift remaining bits to the left
    reader->buffer <<= 1;
    reader->bitCount--;

    return bit;
}
