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
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            printf(" ");
        }
        printf("%02x", data[i]);
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
    for (int i = 0; i < size; ++i) {
        if (data[i] >= 32 && data[i] <= 126) {
            printf("%c", data[i]);
        }
        else {
            printf(".");
        }
    }
}

void printDataAsBinary(unsigned char *data, size_t size) {
    for (int i = 0; i < size; ++i) {
        unsigned char indexChar = data[i];
        int binary[8];
        for (int j = 0; j < 8; ++j) {
            if ((indexChar % 2) == 1) {
                binary[j] = 1;
            }
            else {
                binary[j] = 0;
            }
            indexChar = indexChar / 2;
        }
        printf(" ");
        for (int k = 7; k >= 0; --k) {
            printf("%d", binary[k]);
        }
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
        if (numBytesRead < 16) {
            int diff = 16 - numBytesRead;
            for (int i = numBytesRead; i < 16; ++i) {
                printf("  ");
            }
            int spaces = diff / 2;
            for (int j = 0; j < spaces; ++j) {
                printf(" ");
            }
        }
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
    unsigned char data[6];
    int numBytesRead = fread(data, 1, 6, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsBinary(data, numBytesRead);
        if (numBytesRead < 6) {
            int iter = numBytesRead;
            while (iter < 6) {
                printf("         ");
                ++iter;
            }
        }
        printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, 6, input);
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
