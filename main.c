#include "shell.h"

/**
* main - shell loop to execute commands based on user input
* Return: Always 0 - success
*/
int main(void)
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
	return (0);
}
