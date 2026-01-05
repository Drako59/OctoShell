#pragma once

#include "TypesDef.h"


int pwd(Command* command );
//int ls(int argc, char** argv);
int cd(Command* command);
//int exit(int argc, char** argv);
//int clear(int argc, char** argv);
//int echo(Command* command);





void freePathNode(DirectoryNode* path_pointer);

wchar_t* CreatePath(DirectoryNode* start);


