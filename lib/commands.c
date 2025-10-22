#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

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

BuiltInCommand builtins[] = {
    {"cd", cmd_cd},
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