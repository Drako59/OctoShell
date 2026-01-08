
#include "includes.h"



void print_matrix_wchar(char** matrix, int len) {
	for (int i = 0; i < len; i++) {
		printf(L"%s \n", matrix[i]);
	}
}

BOOL clear(Command* command) {
	COORD cord;
	cord.X = 0;
	cord.Y = 0;
	SetConsoleCursorPosition(command->stdout_file, cord);

	PCSTR sequence = "\x1b[2J\x1b[H";
	int size = strlen(sequence);
	int written;
	WriteFile(command->stdout_file, sequence, size, &written, NULL);

	return written == size;
}

BOOL pwd(Command* command) {
	unsigned int size;
	LPWSTR buffer = (LPWSTR*)malloc(sizeof(LPWSTR) * MAX_PATH_SIZE );
	if (!GetCurrentDirectoryW(MAX_PATH_SIZE - 2,buffer))
		printf("Couldn't get current directory.\n");
	wcsncat(buffer, "\n", MAX_PATH_SIZE);
	size = wcslen(buffer) * sizeof(wchar_t);
	int written;
	
	WriteFile(command->stdout_file, buffer, size, &written, NULL);
	free(buffer);
	return size == written;
}


//has to fix the problex of set a new cd after one and th eproblems of duplicates paths
BOOL cd(Command* command ) {
	
	int argc = command->argc;
	char** argv = command->argv;
	wchar_t* unicode_buffer;
	if (argc == 1) {
		const char* path_to_change = argv[0];
		//const wchar_t* path_to_change =  L"C:\\noamprojects";
		//wprintf(L"%s\n", argv[0]);
		unicode_buffer = utf8_to_utf16(path_to_change);
		wprintf(L"%s\n", unicode_buffer);
		if (SetCurrentDirectoryW(path_to_change) == 0 ){
			free(unicode_buffer);
			printf("The directoy path isn't valid.\n Try an existing path......... \n");
			return 0;
		}
		free(unicode_buffer);
		change_dir_Node(argv[0]);

		/*wchar_t* test = (wchar_t*)malloc(sizeof(wchar_t) * 100);
		wcscpy(test, L"C:\\noamprojects");
		change_dir_Node(test);*/
		free(path);
		path = CreatePath(start_path);
	}
	else {
		printf("Ivalid parameters\n");
		return 0;
	}




	return 1;


}

void freeNode(DirectoryNode* NodePointer) {
	
	free(NodePointer->name);
	free(NodePointer);
}

BOOL change_dir_Node(char* path_to_change) {
	//wprintf(L"%s\n", path_to_change);

	path_to_change =  _strdup(path_to_change);  
	if (!path_to_change) return FALSE;

	const char* sep = "//\\";
	char* token;
	DirectoryNode* New_Node;
	DirectoryNode* last_node;
	char* ptr =NULL;

	DirectoryNode* next_node;
	token = strtok(path_to_change, sep);
	//wprintf(L"%s \n", token);

	if (strcmp(token, "C:") == 0 || strcmp(token, "c:") == 0) {
		freePathNode(start_path);
		char* token_copy = (char*)malloc(sizeof(char*) * 3);
		strcpy(token_copy,"C:");

		//wprintf(L"%s \n", token);

		New_Node = (DirectoryNode*)malloc(sizeof(DirectoryNode));
		if (New_Node == NULL) {
			printf("Malloc allocation failed.\n");
			exit(EXIT_FAILURE);
		}
		New_Node->name = token_copy;
		New_Node->next = NULL;
		New_Node->before = NULL;
		start_path = New_Node;
		path_pointer = start_path;
		token = strtok(NULL, sep);


		//wprintf(L"%s \n", token);

		while (token != NULL) {
			token_copy = _strdup(token);
			New_Node = (DirectoryNode*)malloc(sizeof(DirectoryNode));
			if (New_Node == NULL) {
				printf("Malloc allocation failed.\n");
				exit(EXIT_FAILURE);
			}
			New_Node->next = NULL;
			New_Node->name = token_copy;
			New_Node->before = path_pointer;
			path_pointer->next = New_Node;

			path_pointer = path_pointer->next;
			token = strtok(NULL, sep);
		}
	}
	else if (strcmp(token, "..") == 0  && strcmp(path_pointer->before->name, "C:")) {
		path_pointer = path_pointer->before;
		freeNode(path_pointer->next);
		path_pointer->next = NULL;
	}
	else {

		while (token != NULL)
		{
			New_Node = (DirectoryNode*)malloc(sizeof(DirectoryNode));

			if (New_Node == NULL) {
				printf("Malloc allocation failed.\n");
				exit(EXIT_FAILURE);
			}

			char* token_copy = _strdup(token);

			New_Node->name = token_copy;
			New_Node->next = NULL;
			//wprintf(L"%s \n", token);
			New_Node->before = path_pointer;





			path_pointer->next = New_Node;
			path_pointer = path_pointer->next;
			//wprintf(L"pointer->Name = %s, pointer->Next = %p",path_pointer->name, path_pointer->next);
			token = strtok(NULL, sep);
		}
	}

	free(path_to_change);
	return 1;
	
}


