// grep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CommandFuncs.h"
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#define LINE_SIZE 1024
#define BYTES_BUFFER_SIZE 4096
//typedef struct Flags {
//    BOOL TextFile;
//    BOOL ByteFile;
//} Flags;

typedef enum  Flags {
    TextFile,
    BytesFile,
    AllTextFiles,
    AllBytesFiles,
    None
} Flags;

typedef struct Grep {
    char* text;
    FILE** files;
    size_t filesNumber;
    char** filesNames;
} Grep;

BOOL CmpBytes(char* buffer, size_t bufferSize ,char* pattern) {
    
    size_t patternLength =  strlen(pattern);
    for (size_t i = 0; i < bufferSize - patternLength + 1 ; i++) {
        if (memcmp(buffer + i,pattern,patternLength) == 0) {
            return TRUE;
        }
    }
    return FALSE;

}

//void FlagsInit(Flags* flags) {
//    flags->ByteFile = FALSE;
//    flags->TextFile = FALSE;
//
//
////}
BOOL IsTextFile(LPSTR filename) {
    LPSTR ext = PathFindExtensionA(filename);

    if (ext && _stricmp(ext, ".txt") == 0) return TRUE;
    return FALSE;
}
BOOL BytesGrep(Grep* grepInfo, int i) {
    char buffer[BYTES_BUFFER_SIZE];
    int bytesRead;
    int patternLength = strlen(grepInfo->text);
    if (grepInfo->files[i] != NULL) {
        size_t numberOfBytes = BYTES_BUFFER_SIZE;

        bytesRead = fread(buffer, sizeof(char), numberOfBytes, grepInfo->files[i]);
        if (CmpBytes(buffer, bytesRead, grepInfo->text)) {
            printf("\tFound in: %s\n", grepInfo->filesNames[i]);
            return TRUE;
        }
        size_t tailSize = (patternLength - 1 < bytesRead) ? patternLength - 1 : bytesRead;
        numberOfBytes = BYTES_BUFFER_SIZE - tailSize;

        while ((bytesRead = fread(buffer + tailSize, sizeof(char), numberOfBytes, grepInfo->files[i])) > 0) {
            if (CmpBytes(buffer, bytesRead + tailSize, grepInfo->text)) {
                printf("\tgrep: Found in %s\n", grepInfo->filesNames[i]);
                break;
            }
            memcpy(buffer, buffer + bytesRead - tailSize, tailSize);

        }

        printf("grep: Didn't found.\n");
        return FALSE;
    }
    else {
        printf("grep: didn't succeed opening %s\n", grepInfo->filesNames[i]);
        return FALSE;
    }
}


void TxtGrep(Grep* grepInfo, int i) {
    char buffer[LINE_SIZE];
    BOOL found;
    if (grepInfo->files[i] != NULL) {
        found = FALSE;

        while (fgets(buffer, LINE_SIZE, grepInfo->files[i])) {
            if (strstr(buffer, grepInfo->text)) {
                printf("grep: %s", buffer);
                found = TRUE;
            }
        }
        if (!found) printf("Didn't found any matching in %s\n", grepInfo->filesNames[i]);
    }
    else {
        printf("grep: didn't succeed opening %s\n", grepInfo->filesNames[i]);
    }
}

int main(int argc, char** argv)
{
    //DEBUG
    /*argc = 5;
    argv = (char**)malloc(sizeof(char*) * argc);
    argv[1] = "User";
    argv[2] = "C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\OctoShell\\tests_folder\\tests_files\\test4.txt";
    argv[3] = "-lll";
    argv[4] = "C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\OctoShell\\tests_folder\\tests_files\\test_grep.txt";*/
    //

    CommandParsed command;
    CommandParser_init(&command, argc, argv, NULL);

    char* parametersString = ParameterAsString(command.param, command.param_num);
    
    if (command.argc < 2) {
        printf("grep: Invalid Number of parameters.\n");
        FreeCommandParser(&command);
        return 1;

    }

    Flags Flags = None;
    //FlagsInit(&Flags);


    if (parametersString) {
        if (strchr(parametersString, 'a') || strchr(parametersString, 'A')) {
            Flags = AllTextFiles;
        }
        if (strchr(parametersString, 'b') || strchr(parametersString, 'B')) {
            Flags = AllBytesFiles;
        }
    }
   
    
    
    Grep grepInfo;
    grepInfo.text = command.argv[0];
    grepInfo.filesNumber = command.argc - 1;
    grepInfo.files = (FILE**)malloc(sizeof(FILE*) * (grepInfo.filesNumber));
    grepInfo.filesNames = (char**)malloc(sizeof(char*) * (grepInfo.filesNumber));


    if (grepInfo.files == NULL) {
        printAllocError();
        FreeCommandParser(&command);
        free(parametersString);
        return 1;
    }

    for (int i = 1; i < grepInfo.filesNumber + 1; i++) {

        if (Flags != AllBytesFiles && Flags != AllTextFiles ) {
            if (IsTextFile(command.argv[i])) {
                Flags = TextFile;
            }
            else {
                Flags = BytesFile;
            }
        }
        
        switch (Flags){
                case TextFile: case AllTextFiles:
                    {
                        grepInfo.files[i - 1] = fopen(command.argv[i], "r");
                        grepInfo.filesNames[i - 1] = command.argv[i];

                        
                        TxtGrep(&grepInfo,i - 1);
                        
                        break;
                    }
                case BytesFile: case AllBytesFiles:
                {
                    grepInfo.files[i - 1] = fopen(command.argv[i], "rb");

                    grepInfo.filesNames[i-1] =  command.argv[i];

                    
                    BytesGrep(&grepInfo, i - 1);

                    
            
            

                    break;
                }
        }
        fputs("\n",stdout);
    }





    //Free
    for (int i = 0; i < grepInfo.filesNumber; i++) {
        if (grepInfo.files[i]) {
            fclose(grepInfo.files[i]);
        }
    }
    FreeCommandParser(&command);
    free(grepInfo.files);
    free(parametersString);
}


