# Huffman File Compressor

A small command-line file compressor and decompressor written in C. It uses
Huffman coding and supports text files, arbitrary binary data, empty files, and
files containing only one distinct byte value.

## Build

From the project root, compile the source files with a C compiler:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c src/huffman.c \
	src/file_format.c src/bit_io.c src/heap.c -o compressor
```

To treat compiler warnings as errors, add `-Werror` to the command.

## Usage

Compress a file:

```sh
./compressor compress <input-file> <output.huf>
```

Decompress a `.huf` file:

```sh
./compressor decompress <input.huf> <restored-file>
```

For example, to compress and restore the included text fixture:

```sh
./compressor compress tests/test.txt /tmp/test.huf
./compressor decompress /tmp/test.huf /tmp/restored.txt
cmp tests/test.txt /tmp/restored.txt
```

The program returns a nonzero exit status if the requested operation fails.
The input and output paths for compression must refer to different files.

## Format notes

The compressed file consists of the four-byte `HUF1` magic identifier,
the original size, a 256-entry byte-frequency table, and the Huffman bitstream.
The numeric header values are stored in the host's native representation, so
files are intended for systems with compatible integer representation and byte
order. The bitstream is written most-significant bit first; unused bits in the
last byte are zero-padded. Empty files have a zero original size and an all-zero
frequency table. A file with one distinct byte value is restored by repeating
that value for the original size.

This simple format does not include a checksum. Invalid magic identifiers,
incomplete headers, and truncated bitstreams are rejected, but arbitrary
payload corruption is not guaranteed to be detected if it still decodes to the
recorded original size.

## Tests

The `tests/` directory includes a text fixture and an empty-file fixture. Basic
round-trip checks can be run with the build and usage commands above. For binary
or single-byte input, provide the desired file as the input and compare the
restored output byte-for-byte with `cmp`.
ended