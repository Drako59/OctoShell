// CommandFuncs.cpp : Defines the functions for the static library.
//

#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "framework.h"

// TODO: This is an example of a library function

#include "CommandFuncs.h"





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


//NOTE!!!! CHANE TO BYTE!!!!!


//initilized the command structure with the arguments and parameters (remove the first value of argv because it is the path)
BOOL CommandParser_init(CommandParsed* command, int argc, char** argv) {
	command->argc = 0;
	command->param_num = 0;
	command->argv = (char**)malloc((argc + 1) * sizeof(char*));
	command->param = (char**)malloc((argc + 1) * sizeof(char*));
	//command->parameters = (Parameter*)malloc((argc + 1) * sizeof(Parameter));
	if (command->argv == NULL || command->param == NULL || command->parameters == NULL)
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
			//command->parameters[command->param_num];

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

void FreeCommandParser(CommandParsed* command) {
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

char* ParameterAsString(char** para, int size) {

	if (size == 0) return NULL;
	int length = 1;

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
