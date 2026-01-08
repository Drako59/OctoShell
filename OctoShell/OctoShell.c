// OctoShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "basicImports.h"

#include "includes.h"

//typedef struct DirectoryNode {
//	const wchar_t* name;
//	struct DirectoryNode* next;
//
//} DirectoryNode;
wchar_t* utf8_to_utf16(const char* s)
{
	if (!s) return NULL;

	
	int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
	if (n <= 0) return NULL;

	wchar_t* w = (wchar_t*)malloc((size_t)n * sizeof(wchar_t));
	if (!w) return NULL;

	if (MultiByteToWideChar(CP_UTF8, 0, s, -1, w, n) == 0) {
		free(w);
		return NULL;
	}

	return w; 
}

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
		size += strlen(start->name) + 1;
		start = start->next;
	}
	return size + 1;
}


char* CreatePath(DirectoryNode* start ) {


	int size = size_of_pathW(start);
	char* path = (char*)malloc(sizeof(char) * size);

	strcpy(path,start->name);
	/*wcscat(path, "\\");*/ //BEFORE

	while (start->next) {
		start = start->next;
		strcat(path, "\\");
		strcat(path,start->name);
		//wcscat(path, "\\"); //BEFORE

	}
	strcat(path, "\\"); 
	return path;
}



Command* SepIntoCommand(char* command_str, Command* command) {
	char* ptr = NULL;
	char* cmd;
	int argc = 0;
	HANDLE outFile;
	HANDLE inFile;
	
	wchar_t* unicode_transfer;
	char* sep = " ";
	char* tok;
	//seperate into tokens and 
	tok = strtok(command_str, sep);
	if (!tok) return NULL;
	cmd = tok;
	command->name = (char*)malloc(sizeof(char) * (strlen(tok) + 1));
	strcpy(command->name, cmd);
	//wprintf(L"commandName->%s", command.name); //DEBUG
	tok = strtok(NULL, sep);
	//create a command obj
	while (tok != NULL && command->argc < COMMAND_MAX_SIZE) {
		if (strncmp(tok, ">", COMMAND_MAX_SIZE) == 0 ) {
			tok = strtok(NULL, sep);
			if (tok == NULL)
				break;
			unicode_transfer = utf8_to_utf16(tok);
			outFile = CreateFileW(unicode_transfer, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			free(unicode_transfer);
			if (outFile == NULL || outFile == INVALID_HANDLE_VALUE)
			{
				printf("There was an error opening th file.\n");
				return NULL;
			}
			command->stdout_file = outFile;
			command->redirect_out = TRUE;
			tok = strtok(NULL, sep);
			continue;
		}
		command->argv[argc] = (char*)malloc(sizeof(char) * (strlen(tok) + 2));
		//wprintf(L"argc=%d, tok=\"%ls\", dst=%p\n", argc, tok, command.argv[argc]); //DEBUG
		strcpy(command->argv[argc], tok);
		argc++;
		tok = strtok(NULL, sep);

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




BOOL(*func_arr[])(Command*) = {cd, pwd, echo,clear};
char* funcs_name[] = {"cd","pwd", "echo","clear"};
char* funcs_name_cap[] = { "CD", "PWD","ECHO","CLEAR"};
DirectoryNode* start_path;  

DirectoryNode* path_pointer;
DirectoryNode* before;
char command_str[COMMAND_MAX_SIZE];

char* path;
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
	start_path->name = (char*)malloc(sizeof(char) * 3);
	strcpy(start_path->name,  "C:");
	start_path->next = NULL;
	path_pointer = start_path;

	path = CreatePath(start_path);

	//const wchar_t* path_const = path;
	//const wchar_t* path_const = L"C:\\Users\\User\\source\\repos\\Drako59\\OctoShell\\OctoShell";
	const char* path_const = "C:\\Users\\ayele\\source\\repos\\OctoShell\\OctoShell";
	SetCurrentDirectoryW(path_const);

	//wprintf(L"<%s>", path);
	printf("%s:~", path);
	while (fgets(command_str, COMMAND_MAX_SIZE - 1, stdin)) {
		
		//Set the redirections
		command.stdin_file = hStdInputFile;
		command.stdout_file = hStdOutFile;
		command.redirect_in = FALSE;
		command.redirect_out = FALSE;
		func_match_flag = FALSE;
		
		len = strlen(command_str);
		if (len > 0 && command_str[len - 1] == '\n')
			command_str[len - 1] = L'\0';

		//Remove the additional char fromthe command
		if (strcmp(command_str, "Exit()") == 0	|| strcmp(command_str, "exit") == 0 || strcmp(command_str, "EXIT") == 0 || strcmp(command_str, "exit") == 0)
			break;
		
		if (SepIntoCommand(command_str, &command) == NULL) {
			printf("Invalid Command\n");
			printf("%s:~", path);
			continue;
		}

		//wprintf(L"command->Name: %s, command->argv: %s , command->argc: %d\n", command.name, command.argv[0],command.argc); //DEBUG
		//call the function according to the command
		for (int i = 0; i < sizeof(funcs_name) / sizeof(funcs_name[0]); i++) {
			if (strcmp(funcs_name[i], command.name) == 0 || strcmp(funcs_name_cap[i], command.name) == 0) {
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

		
		printf("%s:~", path);

	}
	
	EXIT:
	freePathNode(start_path);

	printf("success");

	return 0;

}


