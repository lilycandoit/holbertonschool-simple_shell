#include "shell.h"

/**
* _getenv - built getenv manually
* @name: name of the enviroment variable
* Return: the pointer to the value, or NULL if not found
*/

char *_getenv(const char *name)
{
	int i = 0;
	size_t name_len = strlen(name);

	while (environ[i])
	{
		/* compare the start string of environ with given name */
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			/* return pointer to value (after '=') */
			return (&environ[i][name_len + 1]);
		}
		i++;
	}
	return (NULL); /* if not found */
}

/**
* find_command_path - search in PATH for an executable matching command
* @cmd: the command to search for
* Return: Full path to the command, or NULL if not found
*/

char *find_command_path(char *cmd)
{
	char *path_env, *path_copy, *dir;
	char full_path[1024];
	struct stat st;

	if (!cmd)
		return (NULL);

	path_env = _getenv("PATH");
	/* in case PATH is not set or path is empty */
	if (!path_env || *path_env == '\0')
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
	{
		return (NULL);
	}

	dir = strtok(path_copy, ":");
	while (dir)
	{
		/* get the full path*/
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (strdup(full_path));
		}

		dir = strtok(NULL, ":"); /* move to next dir in path*/
	}

	free(path_copy); /* if no match found */
	return (NULL); /* cannot find any path dir */
}


