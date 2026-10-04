#include <stdio.h>

#include "bit_io.h"

int main() {

    FILE *file = fopen(
        "output.bin",
        "wb"
    );

    if (file == NULL) {
        printf("Could not open output file\n");
        return 1;
    }

    BitWriter writer;

    initBitWriter(
        &writer,
        file
    );

    // Write: 100011
    writeBit(&writer, 1);
    writeBit(&writer, 0);
    writeBit(&writer, 0);
    writeBit(&writer, 0);
    writeBit(&writer, 1);
    writeBit(&writer, 1);

    flushBitWriter(&writer);

    fclose(file);

    return 0;
}