#include "shell.h"
#define MAX_ARGS64

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
	int i;
	char *argv[MAX_ARGS64];

	i = 0;
	token = strtok(line, " ");

	while (token != NULL && i < MAX_ARGS64 - 1)
	{

		argv[i++] = token;
		token = strtok(NULL, " ");
	}
	argv[i] = NULL; /* execve needs argv to end with NULL */

	if (argv[0] == NULL)
		return;

	pid = fork(); /* create a new child process */
	if (pid == 0)
	{
		/* replace child with the new program */
		if (execve(argv[0], argv, environ) == -1){
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
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

		if (line[0] == '\0') /* skip empty input */
			continue;

		/* pass the line to handle execution*/
		execute_command(line);
	}
	free(line); /* clean up memory */
}



