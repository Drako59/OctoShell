#include "includes.h";
#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
#endif



BOOL TerminateProcces(int argc,wchar_t* argv[]);

//C:\Users\ayele\source\repos\OctoShell\x64\Debug

//C:\Users\ayele\source\repos\printHI\x64\Debug

//Debug funtion that meant to print a matrix 
void print_matrix(wchar_t* matrix[], int size) {
	for (int i = 0; i < size; i++) {
		wprintf(L"%ls \n", matrix[i]);
	}

}

//Open procces functino
BOOL Open_procces(Command* command ) {
		
	wchar_t* unicode_buffer;
	char** argv = command->argv;
	int argc = command->argc;
	//print_matrix(argv, argc);	//DEBUG
	//printf("argc: %d", argc);//DEBUG
	//set the variable we goonaa use to open the proces
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	
	char* path = command->name;
	
	if(0 == argc) {
		argv = NULL;
	}

	//printf("%p", path);			//DEBUG
	//wprintf(L"%ls", path);		//DEBUG	

	//calculate the size of the coomand to pass
	int size = strlen(path) +  3 + argc ;

	for (int i = 0; i < argc; i++) {
		size += strlen((argv)[i]);
	}

	//Allocate memory for the command to set as a string

	char* para = (wchar_t*)malloc(size * sizeof(wchar_t));

	if (para == NULL) {
		printf("Malloc failed. no availible space in heap.");
		
		return 0;
	}


	//create the command by adding the string as UNICODE using wcsapy and wcscat
	wcscpy(para, L"\"");
	unicode_buffer = utf8_to_utf16(path);
	wcscat(para, unicode_buffer);
	free(unicode_buffer);
	wcscat(para, L"\"");

	//printf("argc: %d\n", argc); //DEBUG
	for (int i = 0; i < argc; i++) {
		if (0 == i)
		{
			//printf("HERE");//DEBUG
			wcscat(para, L" ");
			unicode_buffer = utf8_to_utf16(argv[i]);
			wcscat(para, unicode_buffer);
			free(unicode_buffer);

			
		}
		else {
			//printf("HERE");//DEBUG
			unicode_buffer = utf8_to_utf16(argv[i]);

			wcscat(para, L" ");
			wcscat(para, unicode_buffer);
			free(unicode_buffer);

		}
	}
	//wprintf(L"PARA: %s\n", para); //DEBUG
	//printf("%ls\n", para);		//DEBUG
	
	//Create the procces
	BOOL flag = CreateProcessW(NULL, para,  NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
	if (!flag) {
		//printf("Creating process failed.\n");
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