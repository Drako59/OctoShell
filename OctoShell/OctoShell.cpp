// OctoShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include "includes.h"

int wmain(int argc, wchar_t* argv[])
{
	//print_matrix(argv, argc);
	if (Open_procces(argc - 1, &(argv[1])))
	{
		printf("success\n");
	}
}


