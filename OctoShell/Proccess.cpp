#include "includes.h";
#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
#endif

//C:\Users\ayele\source\repos\OctoShell\x64\Debug

//C:\Users\ayele\source\repos\printHI\x64\Debug

//Debug funtion that meant to print a matrix 
void print_matrix(wchar_t* matrix[], int size) {
	for (int i = 0; i < size; i++) {
		wprintf(L"%ls \n", matrix[i]);
	}

}

BOOL Open_procces(int argc, wchar_t* argv[]) {
	//print_matrix(argv, argc);
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	wchar_t** args;
	wchar_t* path = argv[0];
	if (argc > 1) {
		args = &(argv[1]);
	}
	else {
		args = NULL;
	}

	//printf("%p", path);
	//wprintf(L"%ls", path);

	int size = wcslen(path) + 4;

	for (int i = 0; i < argc - 1; i++) {
		size += wcslen((args)[i]);
	}

	wchar_t* para = (wchar_t*)malloc(size * sizeof(wchar_t));
	wcscpy(para, L"\"");
	wcscat(para, path);
	wcscat(para, L"\"");


	for (int i = 0; i < argc - 1; i++) {
		if (0 == i)
		{
			wcscat(para, L" ");
			wcscat(para, (args)[i]);
			
		}
		else {
			wcscat(para, L" ");
			wcscat(para, (args)[i]);

		}
	}
	//wprintf(L"PARA: %s\n", para);
	//printf("%ls\n", para);
	BOOL flag = CreateProcessW(NULL, para,  NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);

	//printf("%d\n", flag );
	//printf("Here");
	free(para);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return flag;
}