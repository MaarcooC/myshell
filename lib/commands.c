#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"
#include "globals.h"
#include "support.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

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
    printf("Version: %s Author: %s\nGITHUB: %s\nContributors: %s\n", VERSION, AUTHOR, GITHUB, CONTRIBUTORS);
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
    {"unhide", cmd_unhide},
    {"netstats", cmd_net_stats},
    {NULL, NULL} // Sentinel
};

// net stats
void cmd_net_stats (char **args) {
    struct ifaddrs *ifaddr, *ifa;
    char addr[NI_MAXHOST], mask[NI_MAXHOST];
    bool show_mask = (args[1] != NULL && strcmp(args[1], "-m") == 0);

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    printf("\n\033[1;36m[ NETWORK INTERFACES ]\033[0m\n");
        printf("%-10s %-6s %-20s %s\n", "INTERFACE", "TYPE", "IP ADDRESS", show_mask ? "NETMASK" : "");
        printf("----------------------------------------------------------------------\n");

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr || (ifa->ifa_addr->sa_family != AF_INET && ifa->ifa_addr->sa_family != AF_INET6)) 
                continue;

            int family = ifa->ifa_addr->sa_family;
            getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                        addr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            printf("%-10s %-6s %-20s", ifa->ifa_name, (family == AF_INET) ? "IPv4" : "IPv6", addr);
            
            if (show_mask && ifa->ifa_netmask) {
                getnameinfo(ifa->ifa_netmask, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                            mask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                printf(" %s", mask);
            }
            printf("\n");
        }
        freeifaddrs(ifaddr);

    printf("----------------------------------------------------------------------\n");

    print_net_traffic(); // Print traffic statistics
    printf("----------------------------------------------------------------------\n");
    check_connectivity(); // Check connectivity to Google
    printf("----------------------------------------------------------------------\n");
    print_active_connections();
}

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