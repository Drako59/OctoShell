# OctoShell 🐙

OctoShell is a custom Windows command-line shell written in C with the WinAPI.

The project was created to explore low-level operating-system concepts such as command parsing, process creation, pipes, standard-stream redirection, environment variables, aliases, Unicode handling, file-system APIs, and networking APIs.

> OctoShell is an educational project that is still under active development.

---

## Features

### Shell Core

- Custom command tokenizer and parser written in C
- Support for single-quoted and double-quoted arguments
- Multiple commands connected through pipelines
- Output redirection using `>`
- External process execution using `CreateProcessW`
- Standard handle configuration through `STARTUPINFO`
- Pipeline synchronization using `WaitForMultipleObjects`
- UTF-8 and UTF-16 conversion helpers for Windows APIs
- ANSI-colored prompt and OctoShell startup banner
- Custom command executables loaded from a `bin` directory next to `OctoShell.exe`

### Built-in Commands

- `cd`
- `pwd`
- `echo`
- `clear`
- `alias`
- `unalias`
- `unset`

### Environment Variables

OctoShell loads the Windows environment block using `GetEnvironmentStringsW` and stores the variables in its own linked list.

It also supports shell-local assignments and `$VAR` expansion:

```shell
name=Noam
echo $name
unset name
```

Assignments can also appear before a command:

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

Alias expansion is applied to the first word of each command in a pipeline.

### Pipelines

Commands are stored as a linked list of `Command` structures. When the parser encounters `|`, OctoShell creates an anonymous pipe and connects the standard output of one process to the standard input of the next process.

```shell
ls | cat
```

The design supports pipelines containing multiple commands.

---

## Included Commands

OctoShell contains several standalone command executables implemented as separate C projects.

| Command | Description |
|---|---|
| `ls` | Lists directory contents with path support, `-a`, and basic `-l` output |
| `cat` | Reads one or more files and can copy standard input to standard output |
| `cp` | Copies a file to another file or into a directory |
| `ping` | Sends IPv4 ICMP echo requests with DNS resolution, configurable request count, timeout handling, and average RTT |
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

Each `Command` node represents one command. The `next` pointer connects commands inside a pipeline.

---

## Project Structure

```text
OctoShell/
├── OctoShell/
│   ├── OctoShell.c        # Main loop, parsing flow, built-in dispatch, and command execution
│   ├── Parser.c           # Quote-aware tokenization helpers
│   ├── environment.c      # Variables, aliases, unset, and unalias
│   ├── Proccess.c         # Process creation and process resource management
│   ├── DirectoryFunc.c    # cd, pwd, clear, and directory state
│   ├── stringFuncs.c      # String helpers
│   ├── TypesDef.h         # Shared shell structures
│   └── ...
├── CommandFuncs/          # Shared static library for command utilities
├── Cat/                   # cat command
├── cp/                    # cp command
├── ls/                    # ls command
├── ping/                  # IPv4 ICMP ping command
├── nslookup/              # DNS A/PTR lookup command
├── grep/                  # Text and byte-pattern search command
├── http/                  # HTTP command project
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
