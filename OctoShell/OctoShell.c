// OctoShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "basicImports.h"

#include "includes.h"

//typedef struct DirectoryNode {
//	const wchar_t* name;
//	struct DirectoryNode* next;
//
//} DirectoryNode;


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
	/*wcscat(path, "\\");*/ //BEFORE

	while (start->next) {
		start = start->next;
		wcscat(path, "\\");
		wcscat(path,start->name);
		//wcscat(path, "\\"); //BEFORE

	}
	wcscat(path, "\\"); 
	return path;
}



Command* SepIntoCommand(wchar_t* command_str, Command* command) {
	wchar_t* ptr = NULL;
	wchar_t* cmd;
	int argc = 0;
	HANDLE outFile;
	HANDLE inFile;
	

	wchar_t* sep = L" ";
	wchar_t* tok;
	//seperate into tokens and 
	tok = wcstok(command_str, sep, &ptr);
	if (!tok) return NULL;
	cmd = tok;
	command->name = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(tok) + 1));
	wcscpy(command->name, cmd);
	//wprintf(L"commandName->%s", command.name); //DEBUG
	tok = wcstok(NULL, sep, &ptr);
	//create a command obj
	while (tok != NULL && command->argc < COMMAND_MAX_SIZE) {
		if (wcsncmp(tok, L">", COMMAND_MAX_SIZE) == 0 ) {
			tok = wcstok(NULL, sep, &ptr);
			if (tok == NULL)
				break;
			outFile = CreateFileW(tok, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (outFile == NULL || outFile == INVALID_HANDLE_VALUE)
			{
				printf("There was an error opening th file.\n");
				return NULL;
			}
			command->stdout_file = outFile;
			command->redirect_out = TRUE;
			tok = wcstok(NULL, sep, &ptr);
			continue;
		}
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
		if (cmd_pointer->redirect_in)
			CloseHandle(cmd_pointer->stdin_file);
		if (cmd_pointer->redirect_out)
			CloseHandle(cmd_pointer->stdout_file);
		for (int i = 0; i < cmd_pointer->argc; i++) {
			free(cmd_pointer->argv[i]);
		}
		cmd_pointer = cmd_pointer->next;
	}
}

//GLOBAL VARIBALS-----------------------------------------------------------------------------------------------------------------------




BOOL(*func_arr[])(Command*) = {cd, pwd, echo};
wchar_t* funcs_name[] = {L"cd",L"pwd", L"echo" };
wchar_t* funcs_name_cap[] = { L"CD", L"PWD",L"ECHO"};
DirectoryNode* start_path;  

DirectoryNode* path_pointer;
DirectoryNode* before;
wchar_t command_str[COMMAND_MAX_SIZE];

wchar_t* path;
//--------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	HANDLE hStdOutFile = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdInputFile = GetStdHandle(STD_INPUT_HANDLE);
	if ( hStdOutFile == INVALID_HANDLE_VALUE || hStdOutFile == NULL ||
		hStdInputFile == INVALID_HANDLE_VALUE || hStdInputFile == NULL) {
		DWORD e1 = GetLastError();
		printf("hOut=%p lastErr1=%lu \n", hStdOutFile, e1);
		printf("couldn't load stdin and stdout handles\n.");
		return 1;
	}
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

	//const wchar_t* path_const = path;
	//const wchar_t* path_const = L"C:\\Users\\User\\source\\repos\\Drako59\\OctoShell\\OctoShell";
	const wchar_t* path_const = L"C:\\Users\\ayele\\source\\repos\\OctoShell\\OctoShell";
	SetCurrentDirectoryW(path_const);

	//wprintf(L"<%s>", path);
	wprintf(L"%s:~", path);
	while (fgetws(command_str, COMMAND_MAX_SIZE - 1, stdin)) {
		
		//Set the redirections
		command.stdin_file = hStdInputFile;
		command.stdout_file = hStdOutFile;
		command.redirect_in = FALSE;
		command.redirect_out = FALSE;
		func_match_flag = FALSE;
		
		len = wcslen(command_str);
		if (len > 0 && command_str[len - 1] == L'\n')
			command_str[len - 1] = L'\0';

		//Remove the additional char fromthe command
		if (wcscmp(command_str, L"Exit()") == 0	|| wcscmp(command_str, L"exit") == 0 || wcscmp(command_str, L"EXIT") == 0 || wcscmp(command_str, L"exit") == 0)
			break;
		
		if (SepIntoCommand(command_str, &command) == NULL) {
			printf("Invalid Command\n");
			wprintf(L"%s:~", path);
			continue;
		}

		//wprintf(L"command->Name: %s, command->argv: %s , command->argc: %d\n", command.name, command.argv[0],command.argc); //DEBUG
		//call the function according to the command
		for (int i = 0; i < sizeof(funcs_name) / sizeof(funcs_name[0]); i++) {
			if (wcscmp(funcs_name[i], command.name) == 0 || wcscmp(funcs_name_cap[i], command.name) == 0) {
				//command.argv = &(command.argv[1]);
				
				if (func_arr[i](&command) == FALSE)
				{
					printf("There was a problem in function process.");
				};
				func_match_flag = TRUE;
				break;
			}
		}
		
		if (!func_match_flag) {
			Open_procces(&command);
		}
		//free the memory that the command structure used

		FreeCommand(&command);


		//wprintf(L"Command: %s\n", command);

		
		wprintf(L"%s:~", path);

	}
	
	EXIT:
	freePathNode(start_path);

	printf("success");

	return 0;

}


