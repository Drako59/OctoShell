#pragma once

#include "TypesDef.h"


int pwd(Command* command );
//int ls(int argc, char** argv);
int cd(Command* command);
//int exit(int argc, char** argv);
int clear(Command* command);


BOOL change_dir_Node(char* path_to_change);
wchar_t* utf8_to_utf16(const char* s);


void freePathNode(DirectoryNode* path_pointer);

char* CreatePath(DirectoryNode* start);


