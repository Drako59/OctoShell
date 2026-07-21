# OctoShell 🐙

OctoShell is a custom Windows command-line shell written in C using the WinAPI.

The project explores how a shell reads and parses commands, manages its own environment variables and aliases, connects processes through pipes, redirects standard streams, resolves commands, and launches child processes with `CreateProcessW`.

---

## Overview

OctoShell provides a small shell environment for Windows with its own parser, built-in commands, shell state, and external command executables.

The shell processes a command line in several stages:

1. Reads shell-local variable assignments.
2. Expands aliases.
3. Expands environment-variable references.
4. Parses commands, arguments, pipes, and redirection.
5. Executes built-in commands or launches external processes.

---

## Features

### Shell Core

- Custom tokenizer and command parser written in C
- Support for single-quoted and double-quoted arguments
- Multiple commands connected through pipelines
- Output redirection using `>`
- External process execution using `CreateProcessW`
- Standard input, output, and error handle configuration through `STARTUPINFO`
- Anonymous pipes created with `CreatePipe`
- Pipeline synchronization using `WaitForMultipleObjects`
- UTF-8 and UTF-16 conversion helpers for Windows APIs
- ANSI-colored prompt and OctoShell startup banner
- Custom command executables loaded from a `bin` directory next to `OctoShell.exe`

### Built-in Commands

| Command | Description |
|---|---|
| `cd` | Changes the shell's current directory |
| `pwd` | Prints the current directory |
| `echo` | Prints command arguments |
| `clear` | Clears the console |
| `alias` | Creates, updates, or displays aliases |
| `unalias` | Removes aliases |
| `unset` | Removes shell variables |

---

## Environment Variables

OctoShell loads the Windows environment block using `GetEnvironmentStringsW` and stores the variables in its own linked list.

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

Environment variables are represented with the following structure:

```c
typedef struct EnvVar {
    char* name;
    char* value;
    struct EnvVar* nextVar;
} EnvVar;
```

---

## Aliases

Aliases are stored in a linked list and expanded before environment variables are expanded.

```shell
alias ll="ls -l"
ll
unalias ll
```

All aliases can be removed with:

```shell
unalias -a
```

Alias expansion is applied to the first word of each command in a pipeline.

```c
typedef struct AliasVar {
    char* name;
    char* value;
    struct AliasVar* nextVar;
} AliasVar;
```

---

## Pipelines

Commands are stored as a linked list of `Command` structures. When the parser encounters `|`, OctoShell creates an anonymous pipe and connects the standard output of one process to the standard input of the next process.

```shell
ls | cat
```

The linked-list design supports pipelines containing multiple commands.

---

## Included Commands

OctoShell contains several standalone command executables implemented as separate C projects.

| Command | Description |
|---|---|
| `ls` | Lists directory contents and supports path arguments, `-a`, and `-l` |
| `cat` | Reads one or more files and can copy standard input to standard output |
| `cp` | Copies a file to another file or into a directory |
| `ping` | Sends IPv4 ICMP echo requests with DNS resolution, configurable request count, timeout handling, and average round-trip time |
| `nslookup` | Performs DNS A-record lookups and IPv4 reverse PTR lookups |
| `grep` | Searches text files by line and binary files by byte pattern |

The `ping`, `nslookup`, and `grep` projects use the shared `CommandFuncs` static library for common argument parsing and utility functions.

---

## Execution Flow

```text
Read command line
       ↓
Process leading variable assignments
       ↓
Expand aliases
       ↓
Expand environment-variable references
       ↓
Parse commands, arguments, pipes, and redirection
       ↓
Execute built-in commands inside the shell
       ↓
Search the OctoShell bin directory
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

Each `Command` node represents one parsed command. The `next` pointer connects commands inside a pipeline, while the handle fields store the command's standard input and output sources.

---

## Project Structure

```text
OctoShell/
├── OctoShell/
│   ├── OctoShell.c        # Main loop, parsing flow, built-in dispatch, and command execution
│   ├── Parser.c           # Tokenization and quote handling
│   ├── environment.c      # Environment variables, aliases, unset, and unalias
│   ├── Proccess.c         # Process creation and process resource management
│   ├── DirectoryFunc.c    # cd, pwd, clear, and directory state
│   ├── stringFuncs.c      # String utilities
│   ├── ErrorsFunctions.c  # Error output helpers
│   ├── TypesDef.h         # Shared shell structures
│   └── ...
├── CommandFuncs/          # Shared static library for command utilities
├── Cat/                   # cat command
├── cp/                    # cp command
├── ls/                    # ls command
├── ping/                  # IPv4 ICMP ping command
├── nslookup/              # DNS A/PTR lookup command
├── grep/                  # Text and byte-pattern search command
└── OctoShell.sln          # Visual Studio solution
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
- MSVC C/C++ toolchain
- Windows SDK

### Steps

1. Clone the repository:

```shell
git clone https://github.com/Drako59/OctoShell.git
```

2. Open `OctoShell.sln` in Visual Studio.
3. Select an x64 build configuration.
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

---

## Learning Goals

OctoShell was created to study:

- Shell tokenization, parsing, and command expansion
- Windows process creation and process synchronization
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
