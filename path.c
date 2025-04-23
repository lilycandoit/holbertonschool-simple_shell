#include "shell.h"

/**
* find_command_path - search PATH for an executable matching command
* @cmd: the command to search for
* Return: Full path to the command, or NULL if not found
*/

char *find_command_path(char *cmd)
{
    char *path_env, *path_copy, *dir, *full_path;
    size_t len;

    /* if command already includes a slash, try it directly */
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (strdup(cmd));
        else
            return (NULL);
    }

    /* get PATH environment variable */
    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    path_copy = strdup(path_env); /* to be modified later*/
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        len = strlen(dir) + strlen(cmd) + 2; /* 1 for slash and 1 for null terminator */

        full_path = malloc(len);
        if (!full_path)
        {
            free(full_path);
            return (NULL);
        }

        sprintf(full_path, "%s/%s", dir, cmd); /* format the full path*/

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path); /* success */
        }

        free(full_path); /* clean invalid - non excutetable path */
        dir = strtok(NULL, ":"); /* move to next dir in path*/
    }

    free(path_copy); /* if no match found */
    return (NULL); /* cannot find any path dir */
}
