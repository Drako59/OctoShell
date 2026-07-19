//Sperate function by " " which inspierd from strtok. but build for shell's commands
#include "includes.h"


char* strtokCommand(char* command_str, const char* sep) {

	BOOL insideQuote = FALSE;
	BOOL insideDoubleQuote = FALSE;
	static char* command = NULL;
	if (command_str != NULL)
		command = command_str;
	while (command != NULL && *command && strchr(sep, *command))
	{
		command++;
	}
	char* copyCommand = NULL;
	if (command != NULL) {
		int debug = strlen(command);
		for (int i = 0; i < debug; i++) {
			if (!insideQuote && command[i] == '"' && !insideDoubleQuote)
				insideDoubleQuote = TRUE;
			else if (!insideQuote && command[i] == '"' && insideDoubleQuote) {
				insideDoubleQuote = FALSE;

			}
			if (!insideDoubleQuote && command[i] == '\'' && !insideQuote)
				insideQuote = TRUE;
			else if (!insideDoubleQuote && command[i] == '\'' && insideQuote) {
				insideQuote = FALSE;

			}
			if (strchr(sep, command[i]) && !insideDoubleQuote && !insideQuote) {
				command[i] = '\0';
				copyCommand = command;


				//if (*copyCommand != '\0') {
					//i = 0;
				command = &(command[i + 1]);

				/*while (*command != '\0' && strchr(sep, *command)) {
					char* check = strchr(sep, *command);
					command[0] = '\0';
					command = &(command[1]);

				}*/

				//if (*command == '\0') command = NULL;
				return copyCommand;
				//}
			}

		}

		if (insideDoubleQuote || insideQuote) {
			printf("OctoShell: There is must be a matching quote in a parsed string.\n");
			command = NULL;
			return NULL;
		}

		copyCommand = command;
		command = NULL;
		return copyCommand;
	}
	return NULL;


}