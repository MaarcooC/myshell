#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "support.h"

// function to print f1 car form file
void printF1 (void) {
    FILE *f;

    if ((f = fopen("f1.txt", "r")) == NULL ) {
        printf("Error opening file\n");
    } else {
        char c;
        while (!feof(f)) {
            fscanf(f, "%c", &c);
            printf("%c", c);
        }
    }
}

// convert string to lower case
char* string_tolower (char *input) {
    size_t len = strlen(input);

    for (size_t i = 0; i < len; i++) {
        input[i] = tolower((unsigned char)input[i]);
    }

    return input;
}