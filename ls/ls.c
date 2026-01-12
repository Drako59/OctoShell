// ls.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>


#define MAX_PARAMETERS 3
#define elements_in_a_row 3
typedef struct CommandParsed {
	char start_of_hidden_content;
	char** param;
	int param_num;
	int argc;
	char** argv;
} CommandParsed;

void printAllocError(void) {
	fputs("ls: Allocation failed, maybe next time when you will have more ram avilible....\n", stdout);
}

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

char* utf16_to_utf8(const wchar_t* w)
{
	if (!w) return NULL;

	int n = WideCharToMultiByte(CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);
	if (n <= 0) {
		fputs("Threre was an error in converting utf16->utf8\n", stdout); 
		return NULL;
	}
	char* s = (char*)malloc((size_t)n);
	if (!s) { 
		printAllocError(); 
		return NULL;
	}

	if (WideCharToMultiByte(CP_UTF8, 0, w, -1, s, n, NULL, NULL) == 0) {
		free(s);
		fputs("Threre was an error in converting utf16->utf8\n", stdout); 

		return NULL;
	}

	return s; 
}

BOOL CommandParser_init(CommandParsed* command, int argc, char** argv) {
	command->argc = 0;
	command->start_of_hidden_content = L'.';
	command->param_num = 0;
	command->argv = (char**)malloc((argc + 1)* sizeof(char*));
	command->param = (char**)malloc((argc + 1) * sizeof(char*));
	if (command->argv == NULL || command->param == NULL)
	{
		printAllocError();
		if (command->argv != NULL)
			free(command->argv);
		if (command->param != NULL)
			free(command->param);
		return FALSE;
	}
	
	for (int i = 1; i < argc; i++) {
		if (argv[i ][0] == '-' && (strlen(argv[i]) > 1)) {
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
BOOL fputsUTF16(wchar_t* w) {
	char* str = utf16_to_utf8(w);
	if (str == NULL) {
		printAllocError();
		return FALSE;
	}
	fputs(str, stdout);
	free(str);
	return TRUE;

}

char* parameter_as_string(char** para, int size) {

	if (size == 0) return NULL;
	int length = 1;
	//printf("%d", size);//DEBUG
	//printf("%s", para[0]); //DEBUG
	for (int i = 0; i < size; i++) {
		length += strlen(para[i]) - 1 ;
	}

	char* para_str = (char*)malloc(sizeof(char) * length);
	if (para_str == NULL)
	{
		printAllocError();
		return NULL;
	}

	strcpy(para_str, &(para[0][1]));
	for (int i = 1; i < size; i++) {
		strcat(para_str, &(para[0][1]));
	}

	return para_str;

}

char* ls_build_pattern(const char* input) {
	return NULL;
}

BOOL Default_ls(CommandParsed* command) {
	//NOTE: caller must provide pattern(add \ * for directories)
	char* sep = "\n";
	int i = 1;

	char* utf8_filename;
	WIN32_FIND_DATAW fileData;
	HANDLE hSearch;

	if (command->argc == 0) {
		hSearch = FindFirstFileW(L"*", &fileData);
	}
	else {
		wchar_t* WPath = utf8_to_utf16(command->argv[0]);
		hSearch = FindFirstFileW(WPath, &fileData);
		free(WPath);
	}

	if (hSearch == INVALID_HANDLE_VALUE) {
		fputs("There is no file found for this\n", stdout);
		return FALSE;
	}
	if (fileData.cFileName[0] != command->start_of_hidden_content) {
		if (!fputsUTF16(fileData.cFileName)) return FALSE;
	}
	memset(&fileData, 0, sizeof(fileData));
	while (FindNextFileW(hSearch, &fileData)) {
		//sep = i % elements_in_a_row == 0 ? "\n" : "\t\t";
		if (fileData.cFileName[0] != command->start_of_hidden_content) {
			if (!fputsUTF16(fileData.cFileName)) return FALSE;
			fputs(sep, stdout);
		}
		//memset(&fileData, 0, sizeof(fileData));
		i++;
	}
	fputs("\n", stdout);

}

int main(int argc, char** argv)
{
	/*argv = (char*)malloc(sizeof(char*) * 3);
	argc = 1;
	argv[0] = "test";*/
	/*argv = (char**)malloc(sizeof(char*) * 2); //DEBUG
	argv[0] = "test";
	argv[1] = "-a";*/
	//argc = 2;//DEBUG
	char* utf8_filename;
	HANDLE hSearch;
	CommandParsed command;
	if (argc > 2) {
		fputs("ls: Invalid number of parametrs.\n", stdout );
	}
	if (!CommandParser_init(&command, argc, argv)) {
		return 1;
	}
	WIN32_FIND_DATAW fileData;
	int i = 1;
	char* sep = "\n";
	


	char* united_para_str = parameter_as_string(command.param, command.param_num );
		
	//printf("%s\n", united_para_str);
	
	if (united_para_str != NULL && (strchr(united_para_str, "a") == 0)) {
		//printf("%d",command.param_num	);//DEBUG
		command.param_num--;
		command.start_of_hidden_content = L' ';
	}

	if(command.param_num == 0){
		if (Default_ls(&command) == NULL) {
			return 1;
		}
		//if (command.argc == 0) {
		//	hSearch = FindFirstFileW(L"*", &fileData);
		//}
		//else {
		//	wchar_t* WPath = utf8_to_utf16(argv[1]);
		//	hSearch = FindFirstFileW(WPath, &fileData);
		//	free(WPath);
		//}
		//
		//if (hSearch == INVALID_HANDLE_VALUE) {
		//	fputs("There is no file found for this\n", stdout);
		//}
		//if (fileData.cFileName[0] != start_of_hidden_content) {
		//	if (!fputsUTF16(fileData.cFileName)) return 1;
		//}
		//memset(&fileData, 0, sizeof(fileData));
		//while (FindNextFileW(hSearch, &fileData)) {
		//	//sep = i % elements_in_a_row == 0 ? "\n" : "\t\t";
		//	if (fileData.cFileName[0] != start_of_hidden_content) {
		//		if (!fputsUTF16(fileData.cFileName)) return 1;
		//		fputs(sep, stdout);
		//	}
		//	memset(&fileData, 0, sizeof(fileData));
		//	i++;
		//}
		//fputs("\n", stdout);
	}

	if (united_para_str != NULL)
		free(united_para_str);
	wchar_t* test_path = L"C:\\Users\\ayele\\source\\repos\\OctoShell\\OctoShell\\test.txt";
	//wprintf(L"%s, %s \n", fileData.cFileName, fileData.cAlternateFileName);

	return 0;
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
