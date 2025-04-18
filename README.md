# Simple Shell

## Description

This project is a **UNIX command line interpreter** (a simple shell), built as part of the curriculum at Holberton School. It replicates basic functionalities of `/bin/sh` and was built from scratch in C.

The goal was to understand low-level process handling, memory management, and how a shell parses and executes commands in a UNIX-like environment.

---

## Learning Objectives

Throughout this project, we learned:

- How a shell works internally
- Process creation with `fork()` and `execve()`
- Managing environment variables
- String tokenization and memory handling
- Writing built-in commands
- UNIX file permissions and PATH resolution
- Robust error handling

---

## Features

The shell supports the following:

- Displaying a prompt and waiting for user input.
- Reading and parsing input lines.
- Executing commands with full path (`/bin/ls`) or using the `PATH` variable (`ls`).
- Handling command lines with **multiple arguments**.
- Handling the **end-of-file (Ctrl+D)** condition.
- Error handling and output for failed commands.
- Built-in commands:
  - `exit` – exits the shell
  - `env` – prints the current environment variables

---

## Project Structure

```bash
.
├── AUTHORS                # Contributor list
├── README.md              # Project documentation
├── man_1_simple_shell     # Manual file for the shell
├── shell.h                # Header file
├── main.c                 # Entry point
├── shell.c                # Main loop and command execution
├── path.c                 # PATH resolution logic

```
---
## Compilation
To compile all files:
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```
---

## Usage
Run the shell using:
```bash
./hsh
```
Once running, the shell will display a prompt like `cisfun$` and wait for the input.

Examples:
```bash
cisfun$ /bin/ls
cisfun$ ls -l /tmp
cisfun$ env
cisfun$ exit
```

You can also use it in non-interactive mode:
```bash
echo "/bin/ls" | ./hsh
```
---

## Allowed Functions

We were only allowed to use the following system calls and standard library functions:

- `access`, `chdir`, `close`, `execve`, `_exit`, `fork`, `free`, `getcwd`
- `getline`, `getpid`, `isatty`, `kill`, `malloc`, `open`, `opendir`, `read`
- `perror`, `printf`, `putchar`, `strtok`, `wait`, `waitpid`, `write`
- And all functions from `string.h`
