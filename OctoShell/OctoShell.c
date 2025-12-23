// OctoShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "includes.h"

#define COMMAND_MAX_SIZE 256

typedef struct DirectoryNode {
	const wchar_t* name;
	struct DirectoryNode* next;

} DirectoryNode;


int size_of_pathW(DirectoryNode* start) 
{
	int size = 0;
	while ((start)) {
		size += wcslen(start->name) + 1;
		start = start->next;
	}
	return size + 1;
}


wchar_t* createPath(DirectoryNode* start ) {


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


int main()
{
	//print_matrix(argv, argc);

	DirectoryNode start_path_dec = { L"c:" , NULL };
	DirectoryNode* start_path = &start_path_dec;

	DirectoryNode* path_pointer = start_path;
	DirectoryNode* before;
	wchar_t command[COMMAND_MAX_SIZE];

	wchar_t* path;
	path = createPath(start_path);

	const wchar_t* path_const = path;

	SetCurrentDirectoryW(path_const);

	wprintf(L"<%s>", path);

	while (fgetws(command, COMMAND_MAX_SIZE, stdin)) {
		path = createPath(start_path);
		//wprintf(L"%d\n", size_of_pathW(start_path));			//DEBUG
											//DEBUG

		wprintf(L"command: %s\n", command);						//DEBUG
		before = (DirectoryNode*)malloc(sizeof(DirectoryNode));	//DEBUG
		if (before == NULL) {
			printf("Allocation error. \n");						//DEBUG
		}
		before->name = L"ofek";									//DEBUG
		before->next = NULL;
		start_path->next = before;								//DEBUG
		wprintf(L"<%s>", path);

	}
	while ((path_pointer)) {
		before = path_pointer;
		path_pointer = path_pointer->next;
		free(before);
	}

	printf("success");



}


