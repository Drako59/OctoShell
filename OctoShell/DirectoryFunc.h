#pragma once




//int pwd(int argc, char** argv);
//int ls(int argc, char** argv);
int cd(int argc, char** argv);
//int exit(int argc, char** argv);
//int clear(int argc, char** argv);

#ifndef PATH_POINTER
#define PATH_POINTER

typedef struct DirectoryNode {
	const wchar_t* name;
	struct DirectoryNode* next;
	struct DirectoryNode* before;

} DirectoryNode;

extern wchar_t* path;
extern DirectoryNode* start_path;
extern DirectoryNode* path_pointer;
#endif // !PATH_POINTER

#ifndef NodeStruct
#define NodeStruct

#endif // !NodeStruct

void freePathNode(DirectoryNode* path_pointer);

wchar_t* CreatePath(DirectoryNode* start);


