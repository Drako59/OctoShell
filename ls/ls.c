// ls.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <Windows.h>

#define MAX_PARAMETERS 3

typedef struct CommandParsed {
	char* param;
	int param_num;
	int argc;
	char** argv;
} CommandParsed;

void printAllocError(void) {
	printf("ls: Allocation failed, maybe next time when you will have more ram avilible....");
}

BOOL CommandParser(CommandParsed* command, int argc, char** argv) {
	command->argc = 0;
	command->param_num = 0;
	command->argv = (char**)malloc((argc + 1)* sizeof(char*));
	command->param = (char**)malloc((argc + 1) * sizeof(char*));
	if (command->argv == NULL || command->param == NULL)
	{
		printAllocError();
		return FALSE;
	}
	
	for (int i = 1; i < argc; i++) {
		if (argv[i ][0] == '-') {
			command->param[command->param_num] = argv[i ];
			command->param_num++;
		}
		else
		{
			command->argv[command->argc] = argv[i ];
			command->argc++;

		}
	}
	command->argv[command->argc] = NULL;
	command->param[command->param_num] = NULL;

	return TRUE;
}

void FreeCcommandParser(CommandParsed* command) {
	free(command->argv);
	free(command->param);
}

int main(int argc, char** argv)
{
	
	CommandParsed command;
	CommandParser(&command, argc, argv);
	BOOL parameters_inserted = FALSE;

	wchar_t* test_path = L"C:\\Users\\ayele\\source\\repos\\OctoShell\\OctoShell\\test.txt";
	WIN32_FIND_DATAW fileData;
	HANDLE hSearch = FindFirstFileW(test_path, &fileData);
	wprintf(L"%s, %s \n", fileData.cFileName, fileData.cAlternateFileName);

	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
