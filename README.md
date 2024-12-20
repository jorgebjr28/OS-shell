# Shell Project

This project implements a custom shell in C, with features such as executing commands, maintaining a command history, and supporting subshells. The shell is tested against various scenarios to ensure functionality and correctness.

---

## Features

### Command Execution
- Executes commands by searching the system PATH (e.g., `ls`, `/bin/ls`).
- Handles invalid commands with appropriate error messages.

### Command History
- Keeps track of executed commands and displays them via the `history` command.
- Allows re-execution of previous commands using the `!N` syntax, where `N` is the command index.

### Subshells
- Supports creating subshells using the `subshell` command.
- Limits subshell depth to prevent infinite recursion.

### Command Counter
- Displays a counter in the shell prompt that increments with each command.

### Exit Command
- Exits the shell and displays the PID of the exiting process.

---

## Files

### `shell.c`
The main implementation of the shell, featuring:
- Command parsing and execution.
- History management.
- Subshell creation.
- Command counter and prompt management.

### `smp1_tests.c`
The test suite for the shell, containing:
- Tests for valid and invalid command execution.
- Verification of the command counter and history.
- Tests for subshell creation and depth limits.

---

## Compilation

To compile the shell and test suite:
```bash
make
