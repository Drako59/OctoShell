#include "includes.h"



BOOL echo(Command* command) {
	char* buffer;
	int size = 0, written = 0;


	for (int i = 0; i < command->argc; i++) {
		//printf("here");
		size += strnlen(command->argv[i], MAX_PATH_SIZE) + 2;
	}
	size += 4;
	int size2 = size;
	buffer = (char*)calloc(size , sizeof(char));
	if (buffer == NULL)
		return FALSE;
	strncpy(buffer, command->argv[0], size);
		

	for (int i = 1; i < command->argc; i++) {
		strcat(buffer, L" ");
		
		strncat(buffer, command->argv[i], size2);
		

		size2 -= strnlen(command->argv[i], MAX_PATH_SIZE) + 1;
	}
	strncat(buffer, L"\n", size2);
	
	size = size * sizeof(char);;
	/*WORD bom = 0xFEFF;
	WriteFile(command->stdout_file, bom, size, &written, NULL);*/

	WriteFile(command->stdout_file, buffer, size, &written, NULL);
	//wprintf(L"%s", buffer);

	free(buffer);
	return written == size;
}