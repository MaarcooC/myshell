#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"
#include "globals.h"
#include "support.h"

extern bool hide_path; // flag to hide path in prompt

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
    printf("version: %s author: %s\nGITHUB: %s\n", VERSION, AUTHOR, GITHUB);
}

// method to read commands from file and print them on cmd
void cmd_help (char **args) {
    FILE *f;

    // checks file opening
    if ((f = fopen("lib/text/commands.txt", "r")) == NULL) {
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

// method to update and upgrade
void cmd_update(char **args) {
    pid_t pid = fork();

    if (pid == 0) {
        execlp("sudo", "sudo", "apt-get", "update", NULL);
        perror("exec failed");
    } else {
        wait(NULL);

        pid = fork();
        if (pid == 0) {
            execlp("sudo", "sudo", "apt-get", "upgrade", "-y", NULL);
            perror("exec failed");
        } else {
            wait(NULL);
        }
    }
}

// sets a flag to hide the current working directory in the prompt
void cmd_hide (char **args) {
    hide_path = true;
}

// sets a flag to show the current working directory in the prompt
void cmd_unhide (char **args) {
    hide_path = false;
}

BuiltInCommand builtins[] = {
    {"cd", cmd_cd},
    {"version", cmd_version},
    {"help", cmd_help},
    {"up", cmd_update},
    {"hide", cmd_hide},
    {"unhide", cmd_unhide}, // unhide is the same as hide, it just sets the flag to false
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