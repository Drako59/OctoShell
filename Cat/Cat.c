// Cat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


//NOTES: Create a function that open files with auto type of the file. (try diffrent diffult types)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <ctype.h>
//shared function and types
typedef struct CommandParsed {
	char** param;
	int param_num;
	int argc;
	char** argv;
} CommandParsed;

void printAllocError(void) {
	fputs("cat: Allocation failed, maybe next time when you will have more ram avilible....\n", stdout);
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
		fputs("cat: Threre was an error in converting utf16->utf8\n", stdout);
		return NULL;
	}
	char* s = (char*)malloc((size_t)n);
	if (!s) {
		printAllocError();
		return NULL;
	}

	if (WideCharToMultiByte(CP_UTF8, 0, w, -1, s, n, NULL, NULL) == 0) {
		free(s);
		fputs("cat: Threre was an error in converting utf16->utf8\n", stdout);

		return NULL;
	}

	return s;
}


//initilized the command structure with the arguments and parameters (remove the first value of argv because it is the path)
BOOL CommandParser_init(CommandParsed* command, int argc, char** argv) {
	command->argc = 0;
	command->param_num = 0;
	command->argv = (char**)malloc((argc + 1) * sizeof(char*));
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
		if (argv[i][0] == '-' && (strlen(argv[i]) > 1)) {
			command->param[command->param_num] = argv[i];
			command->param_num++;
		}
		else
		{
			command->argv[command->argc] = argv[i];
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
		length += strlen(para[i]) - 1;
	}

	char* para_str = (char*)malloc(sizeof(char) * length);
	if (para_str == NULL)
	{
		printAllocError();
		return NULL;
	}

	strcpy(para_str, &(para[0][1])); //copy the whole string without the - that indicats parameters
	for (int i = 1; i < size; i++) {
		strcat(para_str, &(para[i][1]));
	}

	return para_str;

}
//********************************************************************************************************

void freeFiles(FILE** files, int argc) {
	for (int i = 1; i < argc; i++) {
		fclose(files[i]);
	}
	free(files);
}

int main(int argc, char** argv)
{
	//DEBUG************************************************************************************
	/*argc = 2;

	argv = (char**)malloc(2 * sizeof(char*));

	argv[1] = "C:\\Users\\ayele\\source\\repos\\OctoShell\\OctoShell\\text_test.txt";
	printf("%s", argv[1]);*/
	//END DEBUG********************************************************************************

	

	CommandParsed command;
	CommandParser_init(&command,argc, argv);

	FILE** files = (FILE**)malloc((argc - 1) * sizeof(FILE*));
	if (files == NULL) {
		printAllocError();
		return 1;
	}


	for (int i = 0; i < command.argc; i++) {
		//printf("%s", command.argv[i]);//DEBUG
		files[i] = fopen(command.argv[i], "r");
		if (files[i] == NULL) {
			printf("cat: Error occured while opening %s\n", command.argv[i]);
			perror("cat: Failed to open file");
			freeFiles(files,i);
			return 1;
		}
		
	}

	char buffer[1024];

	for (int i = 0; i < command.argc; i++) {
		//printf("here"); //DEBUG
		while (fgets(buffer,1024,files[i])) {
			//printf("%s", buffer); //DEBUG
			fputs(buffer, stdout);
		}
		printf("\n");
	}
	freeFiles(files, command.argc);

	


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
