# Huffman File Compressor

A small command-line file compressor and decompressor written in C. It uses
Huffman coding and supports text files, arbitrary binary data, empty files, and
files containing only one distinct byte value.

## Prerequisites

You need a C11-compatible compiler (such as GCC or Clang) and GNU Make.

## Build

From the project root, build the program with:

```sh
make
```

This creates the `compressor` executable. To use a different compiler, run
`make CC=clang`. To remove the executable and intermediate build files, run
`make clean`.

## Usage

Compress a file:

```sh
./compressor compress <input-file> <output.huf>
```

Decompress a `.huf` file:

```sh
./compressor decompress <input.huf> <restored-file>
```

For example, create a small input file, then compress and restore it:

```sh
printf 'Example text to compress.\n' > /tmp/input.txt
./compressor compress /tmp/input.txt /tmp/test.huf
./compressor decompress /tmp/test.huf /tmp/restored.txt
cmp /tmp/input.txt /tmp/restored.txt
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

## Testing

No test fixtures are bundled. To check a round trip, use your own input file
with the commands above and compare the original and restored files with
`cmp`. You can also try an empty file (`: > /tmp/empty.txt`), binary data, or a
file containing only one distinct byte value.
ended