#include "includes.h";
#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
#endif



BOOL CloseProcces(int argc,wchar_t* argv[]);

//C:\Users\ayele\source\repos\OctoShell\x64\Debug

//C:\Users\ayele\source\repos\printHI\x64\Debug

//Debug funtion that meant to print a matrix 
void print_matrix(wchar_t* matrix[], int size) {
	for (int i = 0; i < size; i++) {
		wprintf(L"%ls \n", matrix[i]);
	}

}

//Open procces functino
BOOL Open_procces(int argc, wchar_t* argv[]) {
	//print_matrix(argv, argc);		//DEBUG


	//set the variable we goonaa use to open the proces
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

	//printf("%p", path);			//DEBUG
	//wprintf(L"%ls", path);		//DEBUG	

	//calculate the size of the coomand to pass
	int size = wcslen(path) +  2 + argc;

	for (int i = 0; i < argc - 1; i++) {
		size += wcslen((args)[i]);
	}

	//Allocate memory for the command to set as a string

	wchar_t* para = (wchar_t*)malloc(size * sizeof(wchar_t));

	if (para == NULL) {
		printf("Malloc failed. no availible space in heap.");
		return 0;
	}


	//create the command by adding the string as UNICODE using wcsapy and wcscat
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
	//wprintf(L"PARA: %s\n", para); //DEBUG
	//printf("%ls\n", para);		//DEBUG

	//Create the procces
	BOOL flag = CreateProcessW(NULL, para,  NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
	if (!flag) {
		printf("Creating process failed.");
		free(para);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return flag;
	}

	//Wait for the procees to finish before contuine and ending the function
	WaitForSingleObject(pi.hProcess, INFINITE);

	//printf("%d\n", flag );	    //DEBUG
	//printf("Here");				//DEBUG		

	//free the memory we use and close the handle to prevent memory leaks
	free(para);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return flag;
}