#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "support.h"
#include "globals.h"

// function to print f1 car form file
void printF1 (void) {
    FILE *f;

    if ((f = fopen("lib/text/f1.txt", "r")) == NULL ) {
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

// function to return the prompt with username and current working directory
char* print_prompt(char* user, bool hide_path) {
    char* prompt = malloc(MAX_INPUT);
    if (prompt == NULL) {
        perror("malloc");
        exit(1);
    }

    snprintf(prompt, MAX_INPUT, "[%s@myshell", user);
    char cwd[MAX_INPUT];

    // Get the current working directory and print it in the prompt
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (hide_path) strcat(prompt, "]# ");
        else snprintf(prompt + strlen(prompt), MAX_INPUT - strlen(prompt), " ~%s]# ", cwd);
    } else {
        perror("getcwd() error");
    }

    return prompt;
}