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
            return 0;
        str++;
    }
    return 1;
}



/**
* execute_command - take a string and execute it
* @line: raw input line from user
* Steps:
* - fork a child process
* - execute command in child process
* - parent waits for the child to finish
*/
void execute_command(char *line)
{
	char *token;
	pid_t pid;
	int i, j;
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

	if (argv[0] == NULL)
		return;

	/* Check if argv[0] is a full path or needs PATH setup */
	if (strchr(argv[0], '/'))
		cmd_path = strdup(argv[0]);
	else
		cmd_path = find_command_path(argv[0]);

	/* If not found, print error & SKIP fork() */
	if (!cmd_path)
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
		return;
	}

	pid = fork(); /* only fork if command exist */
	if (pid == 0)
	{
		execve(cmd_path, argv, environ);
		perror(argv[0]);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		wait(NULL);
		/* pauses the parent until the child process ends. */
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

	while (1)
	{
		/* if input from terminal (interactive mode) */
		if (isatty(STDIN_FILENO))
			printf("cisfun$ ");

		read = getline(&line, &len, stdin);

		if (read == -1) /* EOF or error */
			break;

		/* Remove newline character */
		if (line[read -1] == '\n' )
			line[read - 1] = '\0';

		/* handle exit command */
		if (strcmp(line, "exit") == 0)
			break;

		/* skip empty input, go back to show prompt again */
		if (line[0] == '\0' || only_spaces(line))
			continue;

		/* pass the line to handle execution*/
		execute_command(line);
	}
	free(line); /* clean up memory */
}



