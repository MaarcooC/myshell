#ifndef BUILTIN_H
#define BUILTIN_H

// Built-in command structure
typedef struct {
    char *name;                  // Command name
    void (*func)(char **args);   // Function pointer to handle the command
} BuiltInCommand;

void cmd_cd(char **args);
void cmd_version(char **args);
int execute_builtin(char **args);

#endif