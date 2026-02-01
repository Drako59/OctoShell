//#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>


typedef struct Parameter {
	char* value;
	char* name;
	BOOL hasValue;
} Parameter;

typedef struct CommandParsed {
	char** param;
	int param_num;
	Parameter* parameters;
	int argc;
	char** argv;
} CommandParsed;



wchar_t* utf8_to_utf16(const char* s);
void printAllocError(void);
BOOL CommandParser_init(CommandParsed* command, int argc, char** argv);
void FreeCommandParser(CommandParsed* command);
BOOL fputsUTF16(wchar_t* w);
char* ParameterAsString(char** para, int size);
