# OctoShell 🐙

OctoShell is an educational Windows command-line shell written in C with the WinAPI.

The project explores how shells parse input, manage shell state, connect processes through pipes, redirect standard streams, expand variables and aliases, resolve commands, and launch child processes with `CreateProcessW`.

> OctoShell is still under active development. It is a low-level learning project rather than a production replacement for CMD, PowerShell, or Bash.

---

## Current Features

### Shell core

- Custom command tokenizer and parser written in C
- Single-quoted and double-quoted arguments
- Multiple commands connected through pipelines
- Output redirection with `>`
- External process creation with `CreateProcessW`
- Standard input/output handle inheritance with `STARTUPINFO`
- Waiting for all processes in a pipeline with `WaitForMultipleObjects`
- UTF-8 and UTF-16 conversion helpers for Windows APIs
- ANSI-colored prompt and OctoShell startup banner
- Custom executables loaded from a `bin` directory next to `OctoShell.exe`

### Built-in commands

- `cd`
- `pwd`
- `echo`
- `clear`
- `alias`
- `unalias`
- `unset`

A small internal `last` command also exists for debugging environment variables.

### Variables

OctoShell loads the Windows environment block with `GetEnvironmentStringsW` and stores the variables in its own linked list.

It also supports shell-local assignments and `$VAR` expansion:

```shell
name=Noam
echo $name
unset name
```

Assignments can appear before a command:

```shell
name=OctoShell echo $name
```

### Aliases

Aliases are stored in a linked list and expanded before environment variables are expanded:

```shell
alias ll="ls -l"
ll
unalias ll
```

All aliases can be removed with:

```shell
unalias -a
```

Alias expansion is applied to the first word of each command inside a pipeline.

### Pipelines

The parser stores commands as a linked list. For every `|`, OctoShell creates an anonymous pipe and connects one process's standard output to the next process's standard input.

```shell
ls | cat
```

The design supports pipelines with more than two commands.

---

## Included Command Projects

OctoShell contains several standalone command executables. These are separate programs rather than wrappers around the Windows commands.

| Command | Current implementation |
|---|---|
| `ls` | Directory listing with basic path support, `-a`, and an experimental `-l` view |
| `cat` | Reads one or more files and can copy piped standard input to standard output |
| `cp` | Copies a file to another file or into a directory |
| `ping` | IPv4 ICMP echo requests, DNS resolution, configurable count with `-n`, timeout handling, and average RTT |
| `nslookup` | DNS A-record lookups and IPv4 reverse PTR lookups |
| `grep` | Searches text files by line and binary files by byte pattern |
| `http` | Planned command; the project currently contains only a placeholder |

`ping`, `nslookup`, and `grep` use the shared `CommandFuncs` static library for common argument parsing and string utilities.

---

## Execution Flow

```text
Read command line
       ↓
Process leading variable assignments
       ↓
Expand aliases
       ↓
Expand $VARIABLE references
       ↓
Parse commands, arguments, pipes, and redirection
       ↓
Run built-in commands inside the shell
       ↓
Try OctoShell's bin directory
       ↓
Launch external commands with CreateProcessW
       ↓
Wait for the pipeline and release process resources
```

---

## Main Command Structure

```c
typedef struct Command {
    char* name;
    int argc;
    BOOL built_in;
    char* argv[COMMAND_MAX_SIZE];
    BOOL redirect_in;
    BOOL redirect_out;
    HANDLE stdin_file;
    HANDLE stdout_file;
    struct Command* next;
} Command;
```

Each node represents one command. The `next` pointer connects commands in a pipeline.

---

## Project Structure

```text
OctoShell/
├── OctoShell/
│   ├── OctoShell.c        # Main loop, parsing flow, built-in dispatch, and command resolution
│   ├── Parser.c           # Quote-aware tokenization helpers
│   ├── environment.c      # Variables, aliases, unset, and unalias
│   ├── Proccess.c         # CreateProcessW and process resource management
│   ├── DirectoryFunc.c    # cd, pwd, clear, and directory state
│   ├── stringFuncs.c      # String helpers
│   ├── TypesDef.h         # Shared shell structures
│   └── ...
├── CommandFuncs/          # Shared static library for command utilities
├── Cat/                   # cat executable
├── cp/                    # cp executable
├── ls/                    # ls executable
├── ping/                  # IPv4 ICMP ping executable
├── nslookup/              # DNS A/PTR lookup executable
├── grep/                  # Text and byte-pattern search executable
├── http/                  # Planned HTTP command placeholder
└── OctoShell.sln
```

---

## Example Usage

```shell
pwd
cd C:\Users\User\Desktop

ls
ls -a
cat file.txt
cp source.txt backup.txt

echo hello world
name=Noam
echo Hello $name
unset name

alias ll="ls -l"
ll
unalias ll

ls | cat
cat file.txt > output.txt

ping -n 3 google.com
nslookup google.com
nslookup -p 8.8.8.8
grep hello file.txt
```

---

## Building

### Requirements

- Windows 10 or Windows 11
- Visual Studio 2022
- MSVC toolchain
- Windows SDK

### Steps

1. Clone the repository:

```shell
git clone https://github.com/Drako59/OctoShell.git
```

2. Open `OctoShell.sln` in Visual Studio.
3. Select an x64 configuration.
4. Build the solution.
5. Place the custom command executables inside a `bin` directory next to `OctoShell.exe`.

```text
OctoShell.exe
bin/
├── ls.exe
├── cat.exe
├── cp.exe
├── ping.exe
├── nslookup.exe
└── grep.exe
```

> Some Visual Studio project settings still contain machine-specific include or library paths. These settings are being replaced with relative solution paths to make clean clones portable.

---

## Current Limitations

- Input redirection with `<` is not active yet.
- `Ctrl+C` is not yet forwarded cleanly to the currently running child process.
- Aliases and shell-local variables exist only for the current session.
- Alias expansion currently performs one expansion pass rather than full recursive expansion.
- Variable expansion does not yet reproduce every Bash quoting rule.
- Explicit PATH resolution is still being improved; part of the search is delegated to Windows process creation.
- `.cmd`, `.bat`, `.com`, and `.ps1` extension attempts are experimental. Script files are not yet launched through their correct interpreter in every case.
- `ls -l` is an early implementation and does not yet provide complete Windows permission and ownership information.
- `grep` currently searches files and does not yet behave like Unix `grep` when receiving only piped standard input.
- The `http` project is currently a placeholder.
- Error handling, resource ownership, and memory cleanup are still being hardened.
- Automated tests have not yet been added.

---

## Near-Term Roadmap

1. Fix remaining undefined behavior, missing return values, and type issues.
2. Remove hard-coded development paths and machine-specific Visual Studio paths.
3. Harden process, pipe, file-handle, and allocation cleanup.
4. Complete input redirection with `<`.
5. Store child exit codes and support `$?`.
6. Add clean `Ctrl+C` handling with Windows console control APIs.
7. Improve command resolution and script-interpreter launching.
8. Add parser, alias, variable-expansion, and pipeline tests.
9. Add persistent configuration through an OctoShell startup file.
10. Later add history, tab completion, `>>`, stderr redirection, `&&`, `||`, and `;`.

---

## Learning Goals

OctoShell was created to study:

- Shell parsing and command expansion
- Windows process creation
- Standard input, output, and error streams
- Anonymous pipes and handle inheritance
- Environment blocks and shell-local state
- Linked lists and dynamic memory management in C
- Unicode handling in Windows programs
- File-system and networking APIs
- Static libraries and multi-project Visual Studio solutions

---

## Author

Created by [Drako59](https://github.com/Drako59) as a low-level C, Windows internals, and systems-programming learning project.
