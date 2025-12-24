
#include "includes.h"



void print_matrix_wchar(wchar_t** matrix, int len) {
	for (int i = 0; i < len; i++) {
		wprintf(L"%s \n", matrix[i]);
	}
}

BOOL cd(int argc, char** argv) {
	
	if (argc == 1) {
		const wchar_t* path_to_change = argv[0];
		//const wchar_t* path_to_change =  L"C:\\noamprojects";
		//wprintf(L"%s\n", argv[0]);
		if (SetCurrentDirectoryW(path_to_change) == 0 ){
			printf("The directoy path isn't valid.\n Try an existing path......... \n");
			return 0;
		}

		change_dir_Node(argv[0]);

		/*wchar_t* test = (wchar_t*)malloc(sizeof(wchar_t) * 100);
		wcscpy(test, L"C:\\noamprojects");
		change_dir_Node(test);*/
		free(path);
		path = CreatePath(start_path);
	}
	else {
		printf("Ivalid parameters");
		return 0;
	}




	return 1;


}

BOOL change_dir_Node(wchar_t* path_to_change) {
	//wprintf(L"%s\n", path_to_change);

	path_to_change =  _wcsdup(path_to_change);  
	if (!path_to_change) return FALSE;

	const wchar_t* sep = L"//\\";
	wchar_t* token;
	DirectoryNode* New_Node;
	DirectoryNode* last_node;
	wchar_t* ptr =NULL;

	DirectoryNode* next_node;
	token = wcstok(path_to_change, sep,&ptr);
	//wprintf(L"%s \n", token);

	if (wcscmp(token, L"C:") == 0) {
		freePathNode(start_path);

		wprintf(L"%s \n", token);

		New_Node = (DirectoryNode*)malloc(sizeof(DirectoryNode));
		New_Node->name = token;
		New_Node->next = NULL;
		start_path = New_Node;
		path_pointer = start_path;
		token = wcstok(NULL, sep, &ptr);
		//wprintf(L"%s \n", token);

		while (token != NULL) {
			New_Node = (DirectoryNode*)malloc(sizeof(DirectoryNode));
			New_Node->next = NULL;
			New_Node->name = token;
			path_pointer->next = New_Node;

			path_pointer = path_pointer->next;
			token = wcstok(NULL, sep, &ptr);
		}
	}
	else {

		while (token != NULL)
		{
			New_Node = (DirectoryNode*)malloc(sizeof(DirectoryNode));
			New_Node->name = token;
			New_Node->next = NULL;
			//wprintf(L"%s \n", token);





			path_pointer->next = New_Node;
			path_pointer = path_pointer->next;
			token = wcstok(NULL, sep, &ptr);
		}
	}

	return 1;
	
}


