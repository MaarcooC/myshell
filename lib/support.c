#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "support.h"
#include "globals.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

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

// Convert bytes to human-readable format
void format_bytes(unsigned long long bytes, char *output) {
    if (bytes >= 1073741824) sprintf(output, "%7.2f GB", (double)bytes / 1073741824);
    else if (bytes >= 1048576) sprintf(output, "%7.2f MB", (double)bytes / 1048576);
    else if (bytes >= 1024) sprintf(output, "%7.2f KB", (double)bytes / 1024);
    else sprintf(output, "%7llu B ", bytes);
}

// Read /proc/net/dev and print RX/TX traffic for each interface
void print_net_traffic() {
    printf("\n\033[1;36m[ TRAFFIC STATISTICS ]\033[0m\n");
    FILE *fp = fopen("/proc/net/dev", "r");
    if (fp) {
        char line[256], rx_fmt[32], tx_fmt[32];
        fgets(line, sizeof(line), fp); fgets(line, sizeof(line), fp); // Skip headers
        
        while (fgets(line, sizeof(line), fp)) {
            char name[32]; unsigned long long rx, tx, d;
            if (sscanf(line, " %[^:]: %llu %llu %llu %llu %llu %llu %llu %llu %llu", name, &rx, &d, &d, &d, &d, &d, &d, &d, &tx) == 10) {
                format_bytes(rx, rx_fmt);
                format_bytes(tx, tx_fmt);
                printf("  %-10s | RX: \033[0;32m%s\033[0m | TX: \033[0;34m%s\033[0m\n", name, rx_fmt, tx_fmt);
            }
        }
        fclose(fp);
    }
}

// check connectivity to Google DNS
void check_connectivity() {
    printf("\n\033[1;36m[ CONNECTIVITY ]\033[0m\n");
    printf("  Internet (8.8.8.8): ");
    fflush(stdout);
    
    int res = system("ping -c 1 -W 1 8.8.8.8 > /dev/null 2>&1");
    if (res == 0) printf("\033[1;32mONLINE\033[0m\n");
    else printf("\033[1;31mOFFLINE\033[0m\n");
}

// Convert hex strings from /proc/net/tcp to human-readable IP
void hex_to_ip(const char *hex, char *ip_out) {
    unsigned int a, b, c, d, port;
    sscanf(hex, "%02X%02X%02X%02X:%04X", &d, &c, &b, &a, &port);
    sprintf(ip_out, "%u.%u.%u.%u:%u", a, b, c, d, port);
}

// Read /proc/net/tcp and print active TCP connections
void print_active_connections() {
    FILE *fp = fopen("/proc/net/tcp", "r");
    if (fp == NULL) {
        perror("Could not open /proc/net/tcp");
        return;
    }

    char line[256];
    // Skip the first line (header)
    fgets(line, sizeof(line), fp);

    printf("\n\033[1;36m[ ACTIVE TCP CONNECTIONS ]\033[0m\n");
    printf("%-22s %-22s %-12s\n", "LOCAL ADDRESS", "REMOTE ADDRESS", "STATE");
    printf("----------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        char local_hex[64], remote_hex[64]; 
        char local_ip[32], remote_ip[32];
        int st;

        // The rest of the sscanf remains the same
        sscanf(line, "%*d: %s %s %x", local_hex, remote_hex, &st);

        hex_to_ip(local_hex, local_ip);
        hex_to_ip(remote_hex, remote_ip);

        // Convert state hex code to readable string
        const char *state_str;
        switch(st) {
            case 0x01: state_str = "\033[0;32mESTABLISHED\033[0m"; break;
            case 0x0A: state_str = "\033[0;33mLISTEN\033[0m";      break;
            default:   state_str = "OTHER";                    break;
        }

        // Only print relevant connections (established or listening)
        if (st == 0x01 || st == 0x0A) {
            printf("%-22s %-22s %-12s\n", local_ip, remote_ip, state_str);
        }
    }
    printf("----------------------------------------------------------------------\n");
    fclose(fp);
}

