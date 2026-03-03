#pragma once
#ifndef COMMAND
#define COMMAND

#define MAX_PATH_SIZE 256
#define COMMAND_MAX_SIZE 256



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

#endif

#ifndef STRING
#define STRING
typedef char* string;
#endif



#ifndef STRING_UNION
#define STRING_UNION
typedef union UString {
	char* utf8;
	wchar_t* unicode;
} UString;

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

#ifdef _WIN32
static void enable_ansi_colors(void) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) return;

	DWORD mode = 0;
	if (!GetConsoleMode(hOut, &mode)) return;

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, mode);
}
#endif

#ifndef FREE_STRUCTURE
#define FREE_STRUCTURE
typedef struct AllocResources {
	
	PROCESS_INFORMATION* pi;
	STARTUPINFO* si;
	HANDLE* processes;
	int processesNum;
} AllocResources;

#endif