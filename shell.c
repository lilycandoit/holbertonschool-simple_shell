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
* execute_command - take a string and execute it
* @line: raw input line from user
* Steps:
* - fork a child process
* - execute command in child process
* - parent waits for the child to finish
*/
void execute_command(char *line, int line_number, int *status_code)
{
	char *token;
	pid_t pid;
	int i, j, status;
	char *cmd_path = NULL;
	char *argv[MAX_ARGS];

	i = 0;
	/* tokenize line into argv[] */
	token = strtok(line, " ");
	while (token != NULL && i < MAX_ARGS - 1)
	{

		argv[i++] = token;
		token = strtok(NULL, " ");
	}
	argv[i] = NULL; /* execve needs argv to end with NULL */

	/* handle built-in env command */
	if (strcmp(argv[0], "env") == 0)
	{
		j = 0;
		while (environ[j])
		{
			printf("%s\n", environ[j]);
			j++;
		}
		return; /* printing done, no fork, no execute */
	}

	/* skip if command is missing or an empty string */
	if (argv[0] == NULL || *argv[0] == '\0')
	{
		return;
	}



	/* Check if argv[0] is a full path or needs PATH setup */
	if (strchr(argv[0], '/'))
		cmd_path = strdup(argv[0]);
	else
		cmd_path = find_command_path(argv[0]);

	if (!cmd_path)
		{
			fprintf(stderr, "./hsh: %d: %s: not found\n", line_number, argv[0]);
			exit(127);
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

/**
* simple_shell - run a basic shell loop to:
* - show promt
* - read user input
* - executing the command
* Return: void
*/

void simple_shell(void)
{
	char *line = NULL; /* store user input */
	size_t len = 0;
	ssize_t read;
	int line_number = 1;
	int status_code = 0;

	while (1)
	{
		/* if input from terminal (interactive mode) */
		if (isatty(STDIN_FILENO))
			printf("cisfun$ ");

		read = getline(&line, &len, stdin);

		if (read == -1) /* EOF or error */
			break;

		/* Remove newline character */
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* handle exit command */
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			exit(status_code); /* exit with latest command status */
		}

		/* skip empty input, go back to show prompt again */
		if (line[0] == '\0' || only_spaces(line))
			continue;
		/* pass the line to handle execution*/
		execute_command(line, line_number, &status_code);
		line_number++;
	}
	free(line); /* clean up memory */
}


