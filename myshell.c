#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "lib/commands.h"
#include "lib/support.h"
#include "lib/globals.h"
#include "lib/linenoise.h"
#include <unistd.h>
#include <termios.h>

bool hide_path = false; // flag to hide path in prompt

int main() {
    char *args[MAX_ARGS];
    char *user = getenv("USER"); // user username

    // prints f1 car in terminal
    printF1();

    // welcome message
    printf("\n\nWelcome. Type ex to exit\n");

    linenoiseHistoryLoad("lib/text/history.txt"); // Load history from file
    linenoiseHistorySetMaxLen(MAX_INPUT); // Set max history length

    char *input = NULL;

    while (1) {
        if(input) free(input);  // it's better to free the memory at the beginning   <---- manages the memory leak of continue 
        char* prompt = print_prompt(user, hide_path); // prints prompt with username and dir
        
        input = linenoise(prompt); // read user input with linenoise
        
        free(prompt); // free allocated memory for prompt    

        string_tolower(input);

        // adds input to history if it's not empty
        if (input && *input != '\0') {
            linenoiseHistoryAdd(input);
        }

        if (strcmp(input, "ex") == 0) { break; }  // <---- Memory leak

        // split input by spaces
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] && i < MAX_ARGS - 1)
            args[++i] = strtok(NULL, " ");
        args[i] = NULL;

        // Check for built-in commands
        if (execute_builtin(args)) {
            continue; // Built-in executed, skip fork/exec   // <---- Memory leak
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Child process executes the command
            execvp(args[0], args);
            perror("execvp");  // Print error if execvp() fails
            exit(1); 
        } else if (pid > 0) {
            // Parent process waits for the child
            waitpid(pid, NULL, 0);
        } else {
            // Error creating the child process
            perror("fork");
        }
    }

    if(input) free(input);  // manages the memory leak of break
    
    linenoiseHistorySave("lib/text/history.txt"); // Save history to file on exit

    return 0;
}
