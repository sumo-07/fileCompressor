#include <stdio.h>
#include <string.h>

#include "file_format.h"

// Initialize the HuffmanHeader with the given original size and frequency table
void initializeHeader(
    HuffmanHeader *header,
    unsigned long long originalSize,
    unsigned long long frequency[256]
) {

    memcpy(
        header->magic,
        MAGIC,
        4
    );

    header->originalSize = originalSize;

    for (int i = 0; i < 256; i++) {

        header->frequency[i] =
            frequency[i];
    }
}

// Write the HuffmanHeader to a file
int writeHeader(
    FILE *file,
    HuffmanHeader *header
) {

    if (fwrite(
            header->magic,
            sizeof(char),
            4,
            file
        ) != 4) {

        return 0;
    }

    if (fwrite(
            &header->originalSize,
            sizeof(unsigned long long),
            1,
            file
        ) != 1) {

        return 0;
    }

    if (fwrite(
            header->frequency,
            sizeof(unsigned long long),
            256,
            file
        ) != 256) {

        return 0;
    }

    return 1;
}

// read the HuffmanHeader from a file and validate it (for decompression)
int readHeader(
    FILE *file,
    HuffmanHeader *header
) {

    if (fread(
            header->magic,
            sizeof(char),
            4,
            file
        ) != 4) {

        return 0;
    }

    if (memcmp(
            header->magic,
            MAGIC,
            4
        ) != 0) {

        return 0;
    }

    if (fread(
            &header->originalSize,
            sizeof(unsigned long long),
            1,
            file
        ) != 1) {

        return 0;
    }

    if (fread(
            header->frequency,
            sizeof(unsigned long long),
            256,
            file
        ) != 256) {

        return 0;
    }

    return 1;
}