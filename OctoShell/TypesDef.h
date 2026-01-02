#pragma once
#ifndef COMMAND
#define COMMAND

#define MAX_PATH_SIZE 256
#define COMMAND_MAX_SIZE 256

typedef struct Command {
	wchar_t* name;
	int argc;
	wchar_t* argv[COMMAND_MAX_SIZE];
	HANDLE stdin_file;
	HANDLE stdout_file;
	struct Command* next;
} Command;

#endif

#ifndef PATH_POINTER
#define PATH_POINTER

typedef struct DirectoryNode {
	const wchar_t* name;
	struct DirectoryNode* next;
	struct DirectoryNode* before;

} DirectoryNode;

extern wchar_t* path;
extern DirectoryNode* start_path;
extern DirectoryNode* path_pointer;
#endif