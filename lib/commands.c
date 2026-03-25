#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

#define VERSION "1.0.2"
#define AUTHOR "Marco"

// function to handle cd built-in command
void cmd_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
}

// method to show myshell version
void cmd_version (char **argsid) {
    printf("version: %s author: %s\n", VERSION, AUTHOR);
}

// method to read commands from file and print them on cmd
void cmd_help (char **args) {
    FILE *f;

    // checks file opening
    if ((f = fopen("lib/commands.txt", "r")) == NULL) {
        printf("Error while opening commands file index\n");
        return;
    } else {
        char line[100];
        while (fgets(line, sizeof(line), f)) {
            printf("%s", line);
        }
    }
    puts(""); // \n
    fclose(f); // closes file
}

BuiltInCommand builtins[] = {
    {"cd", cmd_cd},
    {"version", cmd_version},
    {"help", cmd_help},
    {NULL, NULL} // Sentinel
};

// Function to check and execute built-in commands
int execute_builtin(char **args) {
    if (args[0] == NULL) return 0; // Empty command
    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            builtins[i].func(args);
            return 1; // Built-in executed
        }
    }
    return 0; // Not a built-in
}