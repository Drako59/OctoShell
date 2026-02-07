// grep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CommandFuncs.h"




int main(int argc, char** argv)
{
    CommandParsed command;
    CommandParser_init(&command, argc, argv, NULL);

    char* parametersString = ParameterAsString(command.param, command.param_num);






}


