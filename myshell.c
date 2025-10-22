#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_INPUT 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell# ");
        fflush(stdout);


        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0; // removes \n
        
        if (strcmp(input, "exit") == 0) break;

        // split input by spaces
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] && i < MAX_ARGS - 1)
            args[++i] = strtok(NULL, " ");
        args[i] = NULL;

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

    return 0;
}