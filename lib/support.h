#ifndef SUPPORT_H
#define SUPPORT_H

void printF1 (void);
char* string_tolower (char *input);
char* print_prompt(char* user, bool hide_path);
void format_bytes(unsigned long long bytes, char *output);
void print_net_traffic();
void check_connectivity();
void print_active_connections();

#endif