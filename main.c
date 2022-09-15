#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
    if (argc > 2) {
        printf("Usage: %s [-b|-bits]\n", argv[0]);
        exit(BAD_NUMBER_ARGS);
    }

    if (argc == 2 &&
        (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
        *bits = TRUE;
    } else {
        *bits = FALSE;
    }

    return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
    // The width of the hex dump as always 16-bytes with the extra spaces between the
    // pairs plus an extra space on the end to separate it from section (3) the character representation.
    for (int i = 0; i < size - 1; i += 2) {
        printf(" ");
        printf("%02x%02x", data[i], data[i + 1]);
    }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
    size_t diff = 16 - size;
    int iter = 0;
    if (diff > 0) {
        while (iter < diff) {
            printf("  ");
            ++iter;
        }
        printf("  ");
    }
    for (int i = 0; i < size; ++i) {
        printf("%c", data[i]);
        if (data[i] == "\n") {
            printf(".");
        }
        if (data[i] < 32 || data[i] > 126) {
            printf(".");
        }
    }

}

void printDataAsBinary(unsigned char *data, size_t size) {
    // section (2) only outputs six bytes rather than 16, and bytes are not paired.
    int binary[size * 8];
    for (int i = 0; i < size; ++i) {
        char x = data[i];
        for (int j = 0; j < 8; ++i) {
            if (x % 2 == 1) {
                binary[j * (i + 1)] = 1;
            }
            else {
                binary[j * (i + 1)] = 0;
            }
            x = x / 2;
        }
    }
    for (int k = 0; k < size * 8; ++k) {
        printf("%d", binary[k]);
    }
}

void readAndPrintInputAsHex(FILE *input) {
    unsigned char data[16];
    int numBytesRead = fread(data, 1, 16, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsHex(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, 16, input);
    }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
    unsigned char data[16];
    int numBytesRead = fread(data, 1, 16, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsBinary(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, 16, input);
    }
}

int main(int argc, char **argv) {
    int bits = FALSE;
    FILE *input = parseCommandLine(argc, argv, &bits);

    if (bits == FALSE) {
        readAndPrintInputAsHex(input);
    } else {
        readAndPrintInputAsBits(input);
    }
    return 0;
}
