#ifndef SHELL_H
#define SHELL_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include <sys/stat.h>   /* for stat struct and function  */

extern char **environ;

/* Function prototypes */
int is_whitespace(char c);
int only_spaces(const char *str);
int handle_env_builtin(char **argv);
char *build_argv_and_path(char *line, char **argv);
void execute_command(char *line, int line_number, int *status_code);
char *find_command_path(char *command); 

#endif
