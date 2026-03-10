#include "includes.h";
#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
#endif

#include "TypesDef.h"

//extern int processesNum;
//extern HANDLE* processes;
//extern HANDLE* threads;
BOOL TerminateProcces(int argc,wchar_t* argv[]);



//Debug funtion that meant to print a matrix 
void print_matrix(wchar_t* matrix[], int size) {
	for (int i = 0; i < size; i++) {
		wprintf(L"%ls \n", matrix[i]);
	}

}

//Open procces functino
BOOL Open_procces(Command* command, AllocResources* resources ) {
		

	HANDLE* newProcesses = (HANDLE*)realloc(resources->processes, sizeof(HANDLE) * (resources->processesNum + 1));

	if (NULL == newProcesses) { return FALSE; }

	resources->processes = newProcesses;

	STARTUPINFO* newSi = (STARTUPINFO*)realloc(resources->si, sizeof(STARTUPINFO) * (resources->processesNum + 1));

	if (NULL == newSi) { return FALSE; }

	resources->si = newSi;

	PROCESS_INFORMATION* newPi = (PROCESS_INFORMATION*)realloc(resources->pi, sizeof(PROCESS_INFORMATION) * (resources->processesNum + 1));

	if (NULL == newPi) { return FALSE; }

	resources->pi = newPi;


	int i = resources->processesNum;
	


	wchar_t* unicode_buffer;
	char** argv = command->argv;
	int argc = command->argc;
	//set the variable we are going to  use to open the process
	//STARTUPINFO* si = (STARTUPINFO*)malloc(sizeof(STARTUPINFO));

	
	ZeroMemory(&(resources->si[i]), sizeof(resources->si[i]));
	resources->si[i].cb = sizeof(resources->si[i]);
	resources->si[i].dwFlags = STARTF_USESTDHANDLES;
	//PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)malloc(sizeof(PROCESS_INFORMATION));
	
	char* path = command->name;
	
	if(0 == argc) {
		argv = NULL;
	}


	//calculate the size of the coomand to pass
	int size = strlen(path) +  3 + argc ;

	for (int i = 0; i < argc; i++) {
		size += strlen((argv)[i]);
	}

	//Allocate memory for the command to set as a string
	wchar_t* para = (wchar_t*)malloc(size * sizeof(wchar_t));

	if (para == NULL) {
		printf("Malloc failed. no availible space in heap.\n");
		
		return 0;
	}


	//create the command by adding the string as UNICODE using wcsapy and wcscat
	wcscpy(para, L"\"");
	unicode_buffer = utf8_to_utf16(path);
	wcscat(para, unicode_buffer);
	free(unicode_buffer);
	wcscat(para, L"\"");

	for (int i = 0; i < argc; i++) {
		if (0 == i)
		{
			wcscat(para, L" ");
			unicode_buffer = utf8_to_utf16(argv[i]);
			wcscat(para, unicode_buffer);
			free(unicode_buffer);

			
		}
		else {
			unicode_buffer = utf8_to_utf16(argv[i]);

			wcscat(para, L" ");
			wcscat(para, unicode_buffer);
			free(unicode_buffer);

		}
	}
	
	//Create the procces
	SetHandleInformation(command->stdin_file, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
	SetHandleInformation(command->stdout_file, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
	resources->si[i].hStdInput = command->stdin_file;
	resources->si[i].hStdOutput = command->stdout_file;

	BOOL flag = CreateProcessW(NULL, para,  NULL, NULL, TRUE, 0, NULL, NULL, &(resources->si[i]), &(resources->pi[i]));
	if (!flag) {
		free(para);
		CloseHandle(resources->pi[i].hThread);
		CloseHandle(resources->pi[i].hProcess);
		
		return flag;
	}
	free(para);

	resources->processes[i] = resources->pi[i].hProcess;

	//resources->pi[resources->processesNum - 1] = pi;
	/*resources->si[resources->processesNum - 1] = si;*/

	//resources.threads[resources.processesNum - 1] = pi->hThread;
	 
	//Wait for the procees to finish before contuine and ending the function
	//WaitForSingleObject(pi->hProcess, INFINITE);

	if (command->redirect_in) {
		command->redirect_in = FALSE;
		CloseHandle(command->stdin_file);
	}
	if (command->redirect_out) {
		command->redirect_out = FALSE;
		CloseHandle(command->stdout_file);
	}

	//free the memory we use and close the handle to prevent memory leaks
	/*CloseHandle(pi->hThread);
	CloseHandle(pi->hProcess);*/
	resources->processesNum++;
	return flag;
}

BOOL EndProcesses(AllocResources* resources) {
	WaitForMultipleObjects(resources->processesNum, resources->processes, TRUE, INFINITE);
	for (int i = 0; i < resources->processesNum; i++) {
		CloseHandle(resources->pi[i].hThread);
		CloseHandle(resources->pi[i].hProcess);
		/*free(resources->si[i]);
		free(resources->pi[i]);*/

	}
	resources->processesNum = 0;
	if (resources->processes != NULL) { 
		free(resources->processes); 
	resources->processes = NULL; }

	if (resources->pi != NULL) { free(resources->pi); resources->pi = NULL; }
	if (resources->si != NULL) { free(resources->si); resources->si = NULL; }

}