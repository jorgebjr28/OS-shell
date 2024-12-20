# Shell Project with Subshell and `waitpid` Implementation

This project implements a simple shell program in C that includes the following features:

1. **Subshell Commands**: The shell allows users to create new subshells. Each subshell resets the command history and counter while displaying its process ID.
2. **Command Execution Using `waitpid`**: The shell executes commands by searching the system PATH and uses `waitpid` to ensure the parent process waits for its direct child to finish before returning to the prompt.

The project also includes a `smp1_tests.c` test suite to validate the functionality of the shell implementation.

---

## Features

### Subshell (`subshell` Command)
- Spawns a new subshell with a unique process ID.
- Resets the command history and counter.
- Prevents excessive subshell nesting with a limit, displaying a "Too deep!" message if the limit is exceeded.

### Command Execution
- Executes commands by searching the system PATH.
- Implements `waitpid` for robust process handling, ensuring the shell waits for its direct child process.

### Command History
- Tracks previously executed commands, allowing reruns using syntax like `!1` to rerun the first command.

---

## Running the Shell

### Compilation
Use the following command to compile the project:
```bash
gcc shell.c -o shell
