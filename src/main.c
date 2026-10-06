#include <stdio.h>

#include "file_format.h"

int main() {

    unsigned long long frequency[256] = {0};

    frequency['A'] = 5;
    frequency['B'] = 2;
    frequency['C'] = 1;

    HuffmanHeader header;

    initializeHeader(
        &header,
        8,
        frequency
    );

    FILE *file =
        fopen("test.huf", "wb");

    if (file == NULL) {
        printf("Could not create file\n");
        return 1;
    }

    if (!writeHeader(
            file,
            &header
        )) {

        printf("Failed to write header\n");

        fclose(file);

        return 1;
    }

    fclose(file);

    printf("Header written successfully\n");

    return 0;
}