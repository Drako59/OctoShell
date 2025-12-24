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



BOOL freePathNode(DirectoryNode* pointer) {
	DirectoryNode* before_node;
	//printf("Here");

	int first = 1;
	while ((path_pointer)) {
		printf("HERE");
		before_node = pointer;
		pointer = pointer->next;
		if (!first)
		{
			free(before_node->name);
			first = 0;
		}
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


//GLOBAL VARIBALS-----------------------------------------------------------------------------------------------------------------------


DirectoryNode* start_path;  

DirectoryNode* path_pointer;
DirectoryNode* before;
wchar_t command[COMMAND_MAX_SIZE];

wchar_t* path;
//--------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	//print_matrix(argv, argc);
	int len;
	start_path = (DirectoryNode*)malloc(sizeof(DirectoryNode));
	start_path->name = L"C:";
	start_path->next = NULL;
	path_pointer = start_path;

	path = CreatePath(start_path);

	const wchar_t* path_const = path;

	SetCurrentDirectoryW(path_const);

	wprintf(L"<%s>", path);

	while (fgetws(command, COMMAND_MAX_SIZE - 1, stdin)) {


		len = wcslen(command);
		if (len > 0 && command[len - 1] == L'\n')
			command[len - 1] = L'\0';


		wprintf(L"Command: %s\n", command);
		if (wcscmp(command, L"Exit()") == 0)
			break;

		wchar_t* argv[COMMAND_MAX_SIZE];
		
		argv[0] = (wchar_t*)malloc(sizeof(wchar_t) * 40);
		wcscpy(argv[0], L"C:\\noamprojects\\CTF");

		cd(1, argv);
		wcscpy(argv[0], L"noamprojects");
		cd(1, argv);

		//path = createPath(start_path);
		////wprintf(L"%d\n", size_of_pathW(start_path));			//DEBUG
		//									//DEBUG

		//wprintf(L"command: %s\n", command);						//DEBUG
		//before = (DirectoryNode*)malloc(sizeof(DirectoryNode));	//DEBUG
		//if (before == NULL) {
		//	printf("Allocation error. \n");						//DEBUG
		//}
		//before->name = L"ofek";									//DEBUG
		//before->next = NULL;
		//start_path->next = before;								//DEBUG
		
		
		
		
		
		
		wprintf(L"<%s>", path);

	}

	freePathNode(start_path);

	printf("success");



}


