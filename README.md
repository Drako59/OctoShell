# OctoShell 🐙

OctoShell is a custom Windows shell written in C using the WinAPI.  
The project was built as a low-level learning project that explores how command-line shells parse input, execute processes, handle pipes/redirection, manage environment variables, and launch external command executables.

> This project is still in development and is mainly intended for learning operating-system concepts, process creation, command parsing, and Windows internals.

---

## Overview

OctoShell behaves like a small shell environment on Windows. It reads commands from the user, parses them into an internal `Command` structure, checks for built-in commands, and otherwise tries to launch executables through the Windows process API.

The project includes the main shell plus separate command projects such as `ls`, `cat`, `cp`, `ping`, `nslookup`, `http`, and `grep`.

---

## Features

- Custom command parser written in C
- Support for quoted arguments using single and double quotes
- Built-in shell commands such as:
  - `cd`
  - `pwd`
  - `echo`
  - `clear`
- External command execution using `CreateProcessW`
- Basic pipe support with `CreatePipe`
- Output redirection using `>`
- Shell-local environment variable storage and expansion using `$VAR`
- Loading Windows environment variables through `GetEnvironmentStringsW`
- UTF-8 / UTF-16 conversion helpers for Windows Unicode APIs
- Custom prompt and ANSI-colored terminal output
- Separate command executables loaded from the shell's `bin` directory

---

## Example Usage

```shell
pwd
cd C:\Users\User\Desktop
ls
cat file.txt
echo hello world
name=Noam
echo $name
ls | cat
cat file.txt > output.txt
ping google.com
nslookup google.com
```

---

## Project Structure

```text
OctoShell/
├── OctoShell/          # Main shell project
│   ├── OctoShell.c     # Main loop, parser, variables, command dispatch
│   ├── Proccess.c      # Process creation and process resource handling
│   ├── DirectoryFunc.c # cd, pwd, clear and path handling
│   ├── TypesDef.h      # Main structs: Command, DirectoryNode, EnvVar, etc.
│   └── ...
│
├── Cat/                # cat command
├── cp/                 # cp command
├── ls/                 # ls command
├── ping/               # ping command
├── nslookup/           # DNS lookup command
├── http/               # HTTP-related command project
├── grep/               # grep command project
├── CommandFuncs/       # Additional command utilities
└── OctoShell.sln       # Visual Studio solution
```

---

## Core Design

### Command Parsing

OctoShell parses the user input into a linked list of `Command` objects. Each command stores its name, arguments, redirection handles, and a pointer to the next command in a pipe chain.

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

### Process Execution

External commands are executed through the Windows API with `CreateProcessW`. OctoShell builds a command line, sets standard input/output/error handles through `STARTUPINFO`, and waits for the child processes to finish.

### Pipes

When the parser finds `|`, OctoShell creates a pipe using `CreatePipe`, connects the stdout of one command to the stdin of the next command, and stores the second command in the linked command chain.

### Environment Variables

OctoShell loads the Windows environment variables into its own linked list and also supports shell-local assignments:

```shell
name=OctoShell
echo $name
```

---

## Building

### Requirements

- Windows
- Visual Studio 2022 or newer
- MSVC C/C++ toolchain
- Windows SDK

### Steps

1. Clone the repository:

```shell
git clone https://github.com/Drako59/OctoShell.git
```

2. Open the solution file:

```text
OctoShell.sln
```

3. Build the solution in Visual Studio.

4. Make sure the external command executables are available in a `bin` folder next to `OctoShell.exe`, because the shell searches for custom command executables in that location.

Example layout after build:

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

## Current Limitations

- The shell is not a full Bash or CMD replacement.
- Input redirection with `<` is not fully implemented yet.
- Some command behavior may still be experimental.
- Error handling and memory safety are still being improved.
- The project currently focuses on Windows and WinAPI, not Linux/POSIX.

---

## Learning Goals

This project was created to better understand:

- How shells parse and execute commands
- How processes are created on Windows
- How stdin/stdout redirection works
- How pipes connect multiple processes
- How environment variables can be stored and expanded
- How Unicode and UTF-8/UTF-16 conversions work in Windows programs
- How to structure a larger C project with multiple executables

---

## Future Improvements

- Add better parser support for more Bash-like syntax
- Improve input redirection with `<`
- Add aliases
- Add script support
- Add more shell built-ins
- Improve PATH resolution
- Add better error messages
- Add automated tests
- Improve memory management and cleanup

---

## Author

Created by [Drako59](https://github.com/Drako59) as a low-level C / Windows internals learning project.
