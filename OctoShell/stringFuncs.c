#include "includes.h"



BOOL echo(Command* command) {

	if(command->stdout_file == NULL)
		printf("%s\n", command->argv[0]);

	
}