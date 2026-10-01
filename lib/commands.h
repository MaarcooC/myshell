#ifndef COMMANDS_H
#define COMMANDS_H

// Built-in command structure
typedef struct {
    char *name;                  // Command name
    void (*func)(char **args);   // Function pointer to handle the command
} BuiltInCommand;

void cmd_cd(char **args);
void cmd_version(char **args);
int execute_builtin(char **args);
void cmd_help (char **args);
void cmd_update(char **args);
void cmd_hide (char **args);
void cmd_unhide (char **args);
void cmd_net_stats (char **args);
void cmd_speedtest (char **args);

BuiltInCommand builtins[] = {
    {"cd", cmd_cd},
    {"version", cmd_version},
    {"help", cmd_help},
    {"up", cmd_update},
    {"hide", cmd_hide},
    {"unhide", cmd_unhide},
    {"netstats", cmd_net_stats},
    {"speedtest", cmd_speedtest},
    {NULL, NULL} // Sentinel
};

#endif