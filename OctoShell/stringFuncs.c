#include "includes.h"



BOOL echo(Command* command) {
	char* buffer;
	int size = 0, written = 0;

	if (command->argc < 1) {
		printf("\n");
		return TRUE;
	}

	for (int i = 0; i < command->argc; i++) {
		size += strnlen(command->argv[i], MAX_PATH_SIZE) + 1;
	}
	size += 1;
	int size2 = size;
	buffer = (char*)calloc(size , sizeof(char));
	if (buffer == NULL)
		return FALSE;
	strncpy(buffer, command->argv[0], size);
	

	for (int i = 1; i < command->argc; i++) {
		strcat(buffer, " ");
		strncat(buffer, command->argv[i], size2);
		size2 -= strnlen(command->argv[i], MAX_PATH_SIZE) + 1;
	}
	strncat(buffer, "\n", size2);
	
	size = size * sizeof(char);;
	/*WORD bom = 0xFEFF;
	WriteFile(command->stdout_file, bom, size, &written, NULL);*/

	WriteFile(command->stdout_file, buffer, size - 1, &written, NULL);
	if (command->redirect_in) {
		CloseHandle(command->stdin_file);
		command->redirect_in = FALSE;
	}
	if (command->redirect_out) {
		CloseHandle(command->stdout_file);
		command->redirect_out = FALSE;
	}
	free(buffer);
	return written == (size  - 1);
}