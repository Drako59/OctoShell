// OctoShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "basicImports.h"

#include "includes.h"
#define COMMAND_MAX_SIZE 256

//typedef struct DirectoryNode {
//	const wchar_t* name;
//	struct DirectoryNode* next;
//
//} DirectoryNode;

typedef struct Command {
	wchar_t* name;
	int argc;
	wchar_t* argv[COMMAND_MAX_SIZE];
	HANDLE write_into;
	struct Command* next;
} Command;

void freePathNode(DirectoryNode* pointer) {
	DirectoryNode* before_node;
	//printf("Here");

	int first = 1;
	while ((pointer != NULL)) {
		before_node = pointer;
		pointer = pointer->next;
		//wprintf(L"Node Name->%s \n", before_node->name);
		free(before_node->name);
		free(before_node);
	}


	
}

int size_of_pathW(DirectoryNode* start) 
{
	int size = 0;
	while ((start)) {
		size += wcslen(start->name) + 1;
		start = start->next;
	}
	return size + 1;
}


wchar_t* CreatePath(DirectoryNode* start ) {


	int size = size_of_pathW(start);
	wchar_t* path = (wchar_t*)malloc(sizeof(wchar_t) * size);

	wcscpy(path,start->name);
	wcscat(path, "\\");

	while (start->next) {
		start = start->next;

		wcscat(path,start->name);
		wcscat(path, "\\");

	}

	return path;
}



Command* SepIntoCommand(wchar_t* command_str, Command* command) {
	wchar_t* ptr = NULL;
	wchar_t* cmd;
	int argc = 0;

	wchar_t* sep = L" ";
	wchar_t* tok;
	//seperate into tokens and 
	tok = wcstok(command_str, sep, &ptr);
	if (!tok) return NULL;
	cmd = tok;
	command->name = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(tok) + 1));
	wcscpy(command->name, cmd);
	//wprintf(L"commandName->%s", command.name); //DEBUG

	//create a command obj
	while (tok != NULL && command->argc < COMMAND_MAX_SIZE) {
		command->argv[argc] = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(tok) + 2));
		//wprintf(L"argc=%d, tok=\"%ls\", dst=%p\n", argc, tok, command.argv[argc]); //DEBUG
		wcscpy(command->argv[argc], tok);
		argc++;
		tok = wcstok(NULL, sep, &ptr);

	}

	command->argc = argc;
	command->next = NULL;


}


void FreeCommand(Command* cmd_pointer) {
	while (cmd_pointer != NULL) {
		free(cmd_pointer->name);
		for (int i = 0; i < cmd_pointer->argc; i++) {
			free(cmd_pointer->argv[i]);
		}
		cmd_pointer = cmd_pointer->next;
	}
}

//GLOBAL VARIBALS-----------------------------------------------------------------------------------------------------------------------


BOOL(*func_arr[])(int, char**) = {cd};
wchar_t* funcs_name[] = {L"cd"};

DirectoryNode* start_path;  

DirectoryNode* path_pointer;
DirectoryNode* before;
wchar_t command_str[COMMAND_MAX_SIZE];

wchar_t* path;
//--------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	BOOL func_match_flag = FALSE;
	//print_matrix(argv, argc);
	
	Command command;
	Command* cmd_pointer;
	cmd_pointer = &command;
	//wchar_t** argv;
	int len;
	start_path = (DirectoryNode*)malloc(sizeof(DirectoryNode));
	start_path->name = (wchar_t*)malloc(sizeof(wchar_t) * 3);
	wcscpy(start_path->name,  L"C:");
	start_path->next = NULL;
	path_pointer = start_path;

	path = CreatePath(start_path);

	const wchar_t* path_const = path;

	SetCurrentDirectoryW(path_const);

	wprintf(L"<%s>", path);

	while (fgetws(command_str, COMMAND_MAX_SIZE - 1, stdin)) {
		
		
		func_match_flag = FALSE;
		
		len = wcslen(command_str);
		if (len > 0 && command_str[len - 1] == L'\n')
			command_str[len - 1] = L'\0';

		//Remove the additional char fromthe command
		if (wcscmp(command_str, L"Exit()") == 0)
			break;

		
		if (SepIntoCommand(command_str, &command) == NULL) {
			continue;
		}

		//wprintf(L"command->Name: %s, command->argv: %s , command->argc: %d\n", command.name, command.argv[0],command.argc); //DEBUG
		//call the function according to the command
		for (int i = 0; i < sizeof(funcs_name) / sizeof(funcs_name[0]); i++) {
			if (wcscmp(funcs_name[i], command.name) == 0) {
				//command.argv = &(command.argv[1]);
				command.argc--;
				func_arr[i](command.argc,&command.argv[1]);
				func_match_flag = TRUE;
				break;
			}
		}

		if (!func_match_flag) {
			Open_procces(command.argc, command.argv);
		}
		//free the memory that the command structure used

		FreeCommand(&command);


		//wprintf(L"Command: %s\n", command);

		
		wprintf(L"<%s>", path);

	}

	freePathNode(start_path);

	printf("success");



}


