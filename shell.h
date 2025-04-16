#ifndef SHELL_H
#define SHELL_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"

extern char **environ;

void simple_shell(void);
char *find_command_path(char *cmd);

#endif
