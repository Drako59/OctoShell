//#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


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

char* GetValueOfParameter(CommandParsed* command, char name);

wchar_t* utf8_to_utf16(const char* s);
char* utf16_to_utf8(const wchar_t* w);
void printUtfCastError();
void printAllocError(void);
BOOL CommandParser_init(CommandParsed* command, int argc, char** argv, char* hasParameterValues);
void FreeCommandParser(CommandParsed* command);
BOOL fputsUTF16(wchar_t* w);
char* ParameterAsString(char** para, int size);
BOOL checkForStringParameter(char** param, int size, const char* name);