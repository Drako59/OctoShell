// grep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CommandFuncs.h"

#define LINE_SIZE 1024

typedef struct Flags {
    BOOL TextFile;
    BOOL ByteFile;
} Flags;

typedef struct Grep {
    char* text;
    FILE** files;
    size_t filesNumber;
} Grep;


int main(int argc, char** argv)
{
    ////DEBUG
    //argc = 4;
    //argv = (char**)malloc(sizeof(char*) * argc);
    //argv[1] = "dns";
    //argv[2] = "C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\OctoShell\\tests_folder\\test_dns_ptr.txt";
    //argv[3] = "-a";
    ////

    CommandParsed command;
    CommandParser_init(&command, argc, argv, NULL);

    char* parametersString = ParameterAsString(command.param, command.param_num);
    
    if (command.argc < 2) {
        printf("grep: Invalid Number of parameters.");
        FreeCommandParser(&command);
        return 1;

    }

    Flags Flags;

    if (parametersString) {
        if (strchr(parametersString, 'a') || strchr(parametersString, 'A')) {
            Flags.TextFile = TRUE;
        }
    }
   
    
    Grep grepInfo;
    grepInfo.text = command.argv[0];
    grepInfo.filesNumber = command.argc - 1;
    grepInfo.files = (FILE**)malloc(sizeof(FILE*) * (grepInfo.filesNumber));

    if (grepInfo.files == NULL) {
        printAllocError();

        //FREE METHOD;
        return 1;
    }

    if (Flags.TextFile){
        char buffer[LINE_SIZE];
        for (int i = 1; i < grepInfo.filesNumber + 1; i++) {
            grepInfo.files[i - 1] = fopen(command.argv[i], "r");
        }


        for (int i = 0; i < grepInfo.filesNumber; i++) {
            if (grepInfo.files[i] != NULL) {
                while (fgets(buffer, LINE_SIZE, grepInfo.files[i])) {
                    if (strstr(buffer, grepInfo.text)) {
                        fputs(buffer, stdout);
                    }
                }
            }
        }
    }




    //Free
    for (int i = 0; i < grepInfo.filesNumber; i++) {
        fclose(grepInfo.files[i]);
    }
    free(grepInfo.files);
    free(parametersString);
}


