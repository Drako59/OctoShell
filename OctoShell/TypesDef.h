#pragma once
#ifndef COMMAND
#define COMMAND

#define MAX_PATH_SIZE 256
#define COMMAND_MAX_SIZE 256

typedef struct Command {
	char* name;
	int argc;
	char* argv[COMMAND_MAX_SIZE];
	BOOL redirect_in;
	BOOL redirect_out;
	HANDLE stdin_file;
	HANDLE stdout_file;
	struct Command* next;
} Command;

#endif


#ifndef STRING_UNION
#define STRING_UNION
typedef union string {
	char* utf8;
	wchar_t* unicode;
} string;

#endif
#ifndef PATH_POINTER
#define PATH_POINTER

typedef struct DirectoryNode {
	const char* name;
	struct DirectoryNode* next;
	struct DirectoryNode* before;

} DirectoryNode;

extern char* path;
extern DirectoryNode* start_path;
extern DirectoryNode* path_pointer;
#endif