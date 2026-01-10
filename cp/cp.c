// cp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>



wchar_t* utf8_to_utf16(const char* s)
{
	if (!s) return NULL;


	int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
	if (n <= 0) return NULL;

	wchar_t* w = (wchar_t*)malloc((size_t)n * sizeof(wchar_t));
	if (!w) return NULL;

	if (MultiByteToWideChar(CP_UTF8, 0, s, -1, w, n) == 0) {
		free(w);
		return NULL;
	}

	return w;
}

//FILE* OpenFile() {
//
//}

char* RemoveFileNameFromPath(char* path) {
	char* copy_path = _strdup(path);
	char* lastToken;
	char* token;

	char* parser = "//\\";
	int length = strlen(copy_path);
	char* new_path = (char*)malloc(length * sizeof(char) + 1);
	token = strtok(copy_path,parser);
	strcpy(new_path, "");
	if (token == NULL)
		return NULL;
	lastToken = token;
	token = strtok(NULL, parser);

	while (token != NULL) {
		strcat(new_path, lastToken);
		strcat(new_path, "\\");
		lastToken = token;
		token = strtok(NULL, parser);

	}
	free(copy_path);
	return new_path;
}

int main(int argc, char** argv)
{



	

	

	/*char srcFull[MAX_PATH], dstFull[MAX_PATH];
	GetFullPathNameA(argv[1], MAX_PATH, srcFull, NULL);
	GetFullPathNameA(argv[2], MAX_PATH, dstFull, NULL);

	if (_stricmp(srcFull, dstFull) == 0) {
		printf(stderr, "cp: source and destination are the same file\n");
		return 1;
	}*/
	wchar_t* uni_source;
	wchar_t* uni_destination;
    FILE* source; 
    FILE* destination;
    if (argc < 3) {
        printf("The function takes two arguments at least.\n");
		return 1;
    }
    BOOL parameters_parsed = FALSE;
    if (argv[1][0] ==  '-')
        parameters_parsed = TRUE;
	
	if (!parameters_parsed)
	{
		//printf("here\n"); //DEBUG
		//printf("argv[1]->%s\n", argv[1]);
		//printf("argv[2]->%s\n", argv[2]);

		uni_source = utf8_to_utf16(argv[1]);
		DWORD path_point = GetFileAttributesW(uni_source);
		if (path_point == (path_point & FILE_ATTRIBUTE_DIRECTORY) || path_point == INVALID_FILE_ATTRIBUTES)
		{
			printf("cd: Need a legal path to an existing file.\n");
			return 1;
		}

		source = fopen(argv[1], "rb");

		if (source == NULL) {
			printf("Couldn't open source file due to an error.\n");
			perror("fopen");
			return 1;
		}
		//uni_source = utf8_to_utf16(argv[1]);
		

		uni_destination = utf8_to_utf16(argv[2]);



		
		
		path_point = GetFileAttributesW(uni_destination);

		//check if a dictionary.
		if (path_point ==  (path_point & FILE_ATTRIBUTE_DIRECTORY))
		{
			// check if in the same path:
			wchar_t* srcFullPath[MAX_PATH];
			wchar_t* dstFullPath[MAX_PATH];
			wchar_t* unicode_buffer; 

			char* dir_path = RemoveFileNameFromPath(argv[1]);
			printf("%s\n", dir_path);//DEBUG
			unicode_buffer = utf8_to_utf16(dir_path);
			DWORD check1 = GetFullPathNameW(unicode_buffer, MAX_PATH, srcFullPath, NULL);
			
			free(unicode_buffer);
			unicode_buffer = utf8_to_utf16(argv[2]);
			DWORD check2 = GetFullPathNameW(unicode_buffer, MAX_PATH, dstFullPath, NULL);
			free(unicode_buffer);

			//wprintf(L"%ls\n", srcFullPath);//DEBUG
			//wprintf(L"%ls\n", dstFullPath);//DEBUG


			if (check1 == 0 || check2 == 0 || check1 > MAX_PATH || check2 > MAX_PATH)
			{
				printf("error in getting full path, too large or too small.\n");
			}
			if (_wcsicmp(srcFullPath, dstFullPath) == 0) {
				printf("cp: source and destination are the same file\n");
				return 1;
			}
			//******************


			char* copy_src = _strdup(argv[1]);

			char* parser = "//\\";
			char* token;
			char* last_token;
			token = strtok(copy_src, parser);
			last_token = token;
			while (token != NULL) {
				last_token = token;
				token = strtok(NULL, parser);
			}
			token = last_token;

			int full_path_size = (strlen(token) + strlen(argv[2]) + 2);
			char* full_dest_path = (char*)malloc( full_path_size * sizeof(char) );
			if (full_dest_path == NULL) {
				printf("Allocation failed due to full heap (avalible ram memory). fix it!!!!!!!! \n");
				fclose(source);
				return 1;
			}
			snprintf(full_dest_path, full_path_size, "%s\\%s", argv[2], token);

			destination = fopen(full_dest_path, "wb");
			if (destination == NULL) {

				printf("Couldn't open destination file due to an error.\n");
				free(full_dest_path);
				fclose(source);
				perror("fopen");

				return 1;
			}

			free(full_dest_path);
			free(copy_src);
		}
		/*else if (path_point == INVALID_FILE_ATTRIBUTES) {
			printf("Invalid destination file was set.\n");
			fclose(source);
			return 1;
		}*/
		else {
			destination = fopen(argv[2], "wb");
			if (destination == NULL) {
				printf("Couldn't open destination file due to an error.\n");
				fclose(source);
				perror("fopen");

				return 1;
			}
		}

		char buffer[4096];

		int n;
		n = fread(buffer, sizeof(char), 4096, source);
		int n_write;
		while ( n > 0)
		{
			n_write = fwrite(buffer, sizeof(char) , n, destination);

			if (n_write < n)
			{
				printf("There was a problem in writing into destination file.\n");
			}


			n = fread(buffer, sizeof(char), 4096, source);

		}

		fclose(source);
		fclose(destination);

		//free(uni_source);
		//free(uni_destination); 
		//check if the parameter is a dict
		printf("cp: file copied.\n");

	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
