#ifndef SHELL_H
#define SHELL_H

#define MAX_ARGS 8
#define MAX_ARG_LEN 64

// Simple command argument parser
typedef struct {
    int argc;
    char argv[MAX_ARGS][MAX_ARG_LEN];
} cmd_args_t;

// Parse command string into argc/argv format
cmd_args_t shell_parse_command(char* input);

// Execute built-in command with arguments
int shell_execute_command(cmd_args_t args);

// Help for specific command
void shell_show_help(const char* cmd);

#endif
