#include "includes.h"



BOOL echo(Command* command) {
	wchar_t* buffer;
	int size = 0, written = 0;


	for (int i = 0; i < command->argc; i++) {
		//printf("here");
		size += wcsnlen(command->argv[i], MAX_PATH_SIZE) + 2;
	}
	size += 4;
	int size2 = size;
	buffer = (wchar_t*)calloc(size , sizeof(wchar_t));
	if (buffer == NULL)
		return FALSE;
	wcsncpy(buffer, command->argv[0], size);
		

	for (int i = 1; i < command->argc; i++) {
		wcscat(buffer, L" ");
		
		wcsncat(buffer, command->argv[i], size2);
		

		size2 -= wcsnlen(command->argv[i], MAX_PATH_SIZE) + 1;
	}
	wcsncat(buffer, L"\n", size2);
	
	size = size * sizeof(wchar_t);;
	/*WORD bom = 0xFEFF;
	WriteFile(command->stdout_file, bom, size, &written, NULL);*/

	WriteFile(command->stdout_file, buffer, size, &written, NULL);
	//wprintf(L"%s", buffer);

	free(buffer);
	return written == size;
}