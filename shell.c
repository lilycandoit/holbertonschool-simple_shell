#include "shell.h"
#define MAX_ARGS 64

/**
* is_whitespace - to check whitespace char
* Return: int
*/
int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/**
* only_spaces - to check whitespace input
* Return: 0 if no whitespace, 1 if whitespace
*/
int only_spaces(const char *str)
{
	while (*str)
	{
		if (!is_whitespace(*str))
			return (0);
		str++;
	}
	return (1);
}

/**
* handle_env_builtin - to handle env built-in command
* Return: 0
*/
int handle_env_builtin(char **argv)
{
	int j;

	if (argv[0] && strcmp(argv[0], "env") == 0)
	{
		j = 0;
		while (environ[j])
		{
			printf("%s\n", environ[j]);
			j++;
		}
	}
	return (0);
}

/**
* build_argv_and_path - to tokenize and resolve path
* Return: the command_path
*/
char *build_argv_and_path(char *line, char **argv)
{
	char *token;
	int i = 0;

	token = strtok(line, " ");
	while (token && i < MAX_ARGS - 1)
	{
		argv[i++] = token;
		token = strtok(NULL, " ");
	}
	argv[i] = NULL; /* execve needs argv to end with NULL */

	/* skip if command is missing or an empty string */
	if (argv[0] == NULL || *argv[0] == '\0')
	{
		return (NULL);
	}

	if (strchr(argv[0], '/')) /* if it starts with /, treat it as full path */
		return (strdup(argv[0]));

	return (find_command_path(argv[0])); /* otherwise, search for it in PATH */
}

/**
* execute_command - fork and execute (trimmed version)
* @line: raw input line from user
* @line_number: line number for error message
* @status_code: pointer to store the exit status of the command
* Return: void
*/
void execute_command(char *line, int line_number, int *status_code)
{
	char *cmd_path = NULL;
	char *argv[MAX_ARGS];
	pid_t pid;
	int status;

	cmd_path = build_argv_and_path(line, argv);
	if (!cmd_path) /* if no path found or empty path */
	{
		fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, argv[0]);
		exit(127);
	}

	if (handle_env_builtin(argv))
	{
		free(cmd_path);
		return;
	}
	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, argv, environ);
		perror(argv[0]);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		wait(&status);
		if (WIFEXITED(status))
			*status_code = WEXITSTATUS(status);
		/* to store the status code for exit later */
	}
	else
	{
		perror("fork failed");
	}
	free(cmd_path);
}



