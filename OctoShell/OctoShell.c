// OctoShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "basicImports.h"

#include "includes.h"

#define ESC "\x1b"
#define ESC_END "\x1b[0m"
#define ESC_FORGROUND_END "\x1b[39m"

void SetThemeColors() {
	//printf("\x1b[48;2;48;10;36m");      // רקע
	//printf("\x1b[38;2;248;248;242m");  // טקסט
	printf("\x1b[48;2;48;10;36m");
	printf("\x1b[38;2;248;248;242m");
	printf("\x1b[2J\x1b[H");
	////printf("Ubuntu Style Terminal\n");
}

void EndThemeColors() {
	printf("\x1b[0m");                 // reset

}

void printOctopus() {
	//	printf(  
	//"                         _,--._\n"
	//"                      ,'      `.\n"
	//"              |\\     / ,-.  ,-. \\     /|\n"
	//"              )o),/ ( ( o )( o ) ) \\.(o(\n"
	//"             /o/// /|  `-'  `-'  |\\ \\\\\\o\\\n"
	//"            / / |\\ \\(   .    ,   )/ /| \\ \\\n"
	//"            | | \\o`-/    `\\/'    \\-'o/ | |\n"
	//"            \\ \\  `,'              `.'  / /\n"
	//"         \\.  \\ `-'  ,'|   /\\   |`.  `-' /  ,/\n"
	//"          \\`. `.__,' /   /  \\   \\ `.__,' ,'/\n"
	//"           \\o\\     ,'  ,'    `.  `.     /o/\n"
	//"            \\o`---'  ,'        `.  `---'o/\n"
	//"             `.____,'  -OctoShell  `.____,'\n"
	//);
	//printf(
	//		"                                 __,,----.__\n"
	//		"                            __,,'            ``.\n"
	//		"                      ||\\\\       //  ,--.    ,--.   \\\\       //||\n"
	//		"                      ))oo)),,  ((  ((  oo ))((  oo ))  ))  ,,((oo((\n"
	//		"                     //oo////  ||  ``--''    ``--''    ||  \\\\\\\\oo\\\\\n"
	//		"                   //  // ||\\\\  ((    ..        ,,    ))  //||  \\\\  \\\\\n"
	//		"                   ||  ||  \\\\oo``--    ``\\\\//''      --''oo//  ||  ||\n"
	//		"                   \\\\  \\\\    ``,,                      ``,,    //  //\n"
	//		"              \\\\.      \\\\    ``--    ,,||        /\\\\        ||,,    --''    //      ,,\n"
	//		"                \\\\``..    ``____,,    //        //  \\\\        \\\\    ,,____''    ..''//\n"
	//		"                  \\\\oo\\\\        ,,    ,,        ``..        ,,    ,,        //oo//\n"
	//		"                    \\\\oo``------''      ,,                ,,      ``------''oo//\n"
	//		"                      ``______,,            --OctoShell            ,,______''\n");
	printf(
		ESC "[1;31m"   /* purple */
		"				    __,,----.__\n"
		"			         __,,'            ``.\n"
		"                      ||\\\\       //  ,--.    ,--.   \\\\       //||\n"
		"                      ))oo)),,  ((  ((  oo ))((  oo ))  ))  ,,((oo((\n"
		"                     //oo////  ||  ``--''    ``--''    ||  \\\\\\\\oo\\\\\n"
		"                   //  // ||\\\\  ((    ..        ,,    ))  //||  \\\\  \\\\\n"
		"                   ||  ||  \\\\oo``--    ``\\\\//''      --''oo//  ||  ||\n"
		"                   \\\\  \\\\    ``,,                      ``,,    //  //\n"
		"              \\\\.      \\\\    ``--    ,,||        /\\\\        ||,,    --''    //      ,,\n"
		"                \\\\``..    ``____,,    //        //  \\\\        \\\\    ,,____''    ..''//\n"
		"                  \\\\oo\\\\        ,,    ,,        ``..        ,,    ,,        //oo//\n"
		"                    \\\\oo``------''      ,,                ,,      ``------''oo//\n"
		ESC "[1;36m"   /* cyan */
		"                      ``______,,            --OctoShell            ,,______''\n"
		ESC "[0m"      /* reset */
	);
	
	
}

//DEBUG FUNCTIONS
void printEnvVars(EnvVar* vars) {
	for (EnvVar* pCur = vars; pCur != NULL; pCur = pCur->nextVar)
	{
		printf("%s=%s\n", pCur->name,pCur->value);
	}
}

void printLastEnvVar(Command* command);


//
//Alocation error
void printAllocationError(void) {
	printf("OctoShell: There is no available space in heap. try to upgrade your pc maybe?(pls?) :(\n");

}

void FreeCommand(Command* cmd_pointer) {
	Command* current;
	BOOL first = TRUE;
	while (cmd_pointer != NULL) {
		if(cmd_pointer->name)
			free(cmd_pointer->name);
		if (cmd_pointer->redirect_in)
			CloseHandle(cmd_pointer->stdin_file);
		if (cmd_pointer->redirect_out)
			CloseHandle(cmd_pointer->stdout_file);
		for (int i = 0; i < cmd_pointer->argc; i++) {
			free(cmd_pointer->argv[i]);
		}
		current = cmd_pointer;
		cmd_pointer = cmd_pointer->next;
		if (!first)
			free(current);
		else first = FALSE;
	}
}

void FreeEnvVars(EnvVar* envVars) {
	EnvVar* current;
	while (envVars != NULL) {
		free(envVars->name);
		free(envVars->value);
		current = envVars;
		envVars = envVars->nextVar;
		free(current);
	}

}

char* utf16_to_utf8(const wchar_t* w)
{
	if (!w) return NULL;

	int n = WideCharToMultiByte(CP_UTF8, 0, w, -1, NULL, 0, NULL, NULL);
	if (n <= 0) {
		fputs("Threre was an error in converting utf16->utf8\n", stdout);
		return NULL;
	}
	char* s = (char*)malloc((size_t)n);
	if (!s) {
		printAllocationError();
		return NULL;
	}

	if (WideCharToMultiByte(CP_UTF8, 0, w, -1, s, n, NULL, NULL) == 0) {
		free(s);
		fputs(" Threre was an error in converting utf16->utf8\n", stdout);

		return NULL;
	}

	return s;
}

wchar_t* utf8_to_utf16(const char* s)
{
	if (!s) return NULL;


	int n = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
	if (n <= 0) return NULL;

	wchar_t* w = (wchar_t*)malloc((size_t)n * sizeof(wchar_t));
	if (!w) return NULL;

	if (MultiByteToWideChar(CP_UTF8, 0, s, -1, w, n) == 0) {
		free(w);
		return NULL;
	}

	return w;
}

//free the memeory that was allcoated the nodes of the path and the allocated memeory for it's content.
void freePathNode(DirectoryNode* pointer) {
	DirectoryNode* before_node;

	int first = 1;
	while ((pointer != NULL)) {
		before_node = pointer;
		pointer = pointer->next;
		//wprintf(L"Node Name->%s \n", before_node->name);
		free(before_node->name);
		free(before_node);
	}



}

//return the size required to create the path.
int size_of_path(DirectoryNode* start)
{
	int size = 0;
	while ((start)) {
		size += strlen(start->name) + 1;
		start = start->next;
	}
	return size + 1;
}

//copy a string from heap and return a new pointer to heap with the same initilize value
BOOL StrHeapCpy(char** pDst, char* pSrc) {
	if (!pSrc) {
		*pDst = NULL;
		return TRUE;
	}

	size_t NumOfBytes = strlen(pSrc) * sizeof(char) + sizeof(char);
	*pDst = (char*)malloc(sizeof(char) * NumOfBytes);

	if (*pDst == NULL) {
		printAllocationError();
		return NULL;
	}
	memcpy(*pDst, pSrc, NumOfBytes);
	return TRUE;
}


//return a char* to a path created according to the node who represents the file system.
char* CreatePath(DirectoryNode* start) {


	int size = size_of_path(start);
	char* path = (char*)malloc(sizeof(char) * size);

	strcpy(path, start->name);
	/*wcscat(path, "\\");*/ //BEFORE

	while (start->next) {
		start = start->next;
		strcat(path, "\\");
		strcat(path, start->name);
		//wcscat(path, "\\"); //BEFORE

	}
	strcat(path, "\\");
	return path;
}

//intilize the command structure
void Command_init(Command* command, HANDLE hStdInputFile, HANDLE hStdOutFile) {
	command->stdin_file = hStdInputFile;
	command->stdout_file = hStdOutFile;
	command->redirect_in = FALSE;
	command->redirect_out = FALSE;
	command->built_in = FALSE;
}

//create a copy of the command structure. (deepcopy only the name).
Command* CopyCommand(Command* command) {
	Command* command_copy = (Command*)malloc(sizeof(Command) * 1);

	if (command_copy == NULL) {
		printAllocationError();
		return NULL;
	}

	if (StrHeapCpy(&(command_copy->name), command->name) == NULL) {
		free(command_copy);
		return NULL;
	}
	memcpy(command_copy->argv, command->argv, sizeof(command->argv));
	command_copy->argc = command->argc;
	command_copy->redirect_in = command->redirect_in;
	command_copy->redirect_out = command->redirect_out;
	command_copy->stdin_file = command->stdin_file;
	command_copy->stdout_file = command->stdout_file;
	command_copy->next = NULL;

	return command_copy;



}

void FreeStrArr(char** arr, int argc) {
	for (int i = 0; i < argc; i++) {
		free(arr[i]);
	}
	free(arr);

}

BOOL SetRedirectOUT(char* fileName, Command* command) {
	wchar_t* unicode_transfer = utf8_to_utf16(fileName);
	HANDLE outFile = CreateFileW(unicode_transfer, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	free(unicode_transfer);
	if (outFile == NULL || outFile == INVALID_HANDLE_VALUE)
	{
		printf("There was an error opening th file.\n");
		FreeStrArr(command->argv, command->argc);
		free(command->name);
		return NULL;
	}
	command->stdout_file = outFile;
	command->redirect_out = TRUE;
}
BOOL SetRedirectINPUT(char* fileName, Command* command) {
	wchar_t* unicode_transfer = utf8_to_utf16(fileName);
	HANDLE inputFile = CreateFileW(unicode_transfer, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	free(unicode_transfer);
	if (inputFile == NULL || inputFile == INVALID_HANDLE_VALUE)
	{
		printf("There was an error opening th file.\n");
		FreeStrArr(command->argv, command->argc);
		free(command->name);
		return NULL;
	}
	command->stdin_file = inputFile;
	command->redirect_in = TRUE;
}

BOOL CheckForQuote(char* command_str, char sep) {
	int QuoteCounts = 0;
	for (int i = 0; i < strlen(command_str); i++) {
		if (command_str[i] == sep) QuoteCounts++;
	}

	return QuoteCounts / 2;
}


//to upgrade
enum QState {
	QNone,
	QDouble,
	QSingle
};





void FreeCommandSepError() {

}

char* GetEnvValue(EnvVar* envVars, char* name) {
	while (envVars) {
		if (strcmp(envVars->name, name) == 0) {
			return _strdup(envVars->value);
		}
		envVars = envVars->nextVar;
	}
	return _strdup("");
}

char* PlaceEnvVars(char* arg_par, EnvVar* envVars) { 
	char* sep = "$";
	char* arg = _strdup(arg_par);
	if (arg == NULL) return NULL;
	char* tok = strtok(arg,sep);
	char* placed_arg = NULL;
	char* placed_arg_alloc = NULL;

	char* end_var = NULL;

	char buffer_var_name[MAX_PATH];
	char* value = NULL;
	int tok_len;
	int var_len;
	int total_len = 0;
	BOOL first_is_var = FALSE;
	if (arg_par[0] == '$') first_is_var = TRUE;
	BOOL first = TRUE;
	while (tok != NULL) {
		var_len = 0;
		tok_len = strlen(tok);
		for (int i = 0; i < tok_len; i++) {
			if (!((tok[i] <= '9' && tok[i] >= '0') || (tok[i] <= 'Z' && tok[i] >= 'A') || ( tok[i] <= 'z' && tok[i] >= 'a') || (tok[i] == '_'))) {
				var_len = i;
				end_var = tok + i;
				break;
			}
			if (i == tok_len - 1) {
				var_len = i  + 1;
				end_var = tok + i + 1;
			}
			
		}
		snprintf(buffer_var_name, var_len + 1, "%s", tok);
		if(first)
			value = first_is_var ? GetEnvValue(envVars, buffer_var_name) : _strdup("");
		else
			value = GetEnvValue(envVars, buffer_var_name);


		if (value == NULL) {
			if (placed_arg != NULL) free(placed_arg);
			return NULL;
		}
		if(first)
			total_len += first_is_var? tok_len - var_len + strlen(value) : tok_len;
		else
			total_len += tok_len - var_len + strlen(value);

		placed_arg_alloc = (char*)realloc(placed_arg, sizeof(char) * (total_len + 1));
		if (placed_arg_alloc == NULL) {
			if (placed_arg != NULL) free(placed_arg);
			free(value);
			return NULL;
		}
		placed_arg = placed_arg_alloc;
		if (first) {
			if (first_is_var) {
				strcpy(placed_arg, value);
				strcat(placed_arg, end_var);
			}
			else {
				strcpy(placed_arg, tok);
			}
			first = FALSE;
		}
		else
		{
			strcat(placed_arg, value);
			strcat(placed_arg, end_var);

		}
		free(value);
		tok = strtok(NULL,sep);
	}
	free(arg);
	return placed_arg;  //retun an allocation to heap so rememver to free.
}


BOOL IsAssignment(char* token) {
	if (strchr(token, '=')) {
		return TRUE;
	}
	return FALSE;
}

BOOL CheckVarName(char* name) {
	if (name[0] >= '0' && name[0] <= '9') {
		return FALSE;
	}
	int length = strchr(name, '=') - name;
	for (int i = 0; i < length; i++) {
		if (!((name[i] <= '9' && name[i] >= '0') || (name[i] <= 'Z' && name[i] >= 'A') || (name[i] <= 'z' && name[i] >= 'a') || (name[i] == '_'))) {
			return FALSE;
		}
	}
	return TRUE;
}

//take the original text and remove the quotes
BOOL RemoveQuotes(char* text) {
	int len = strlen(text);
	int p = 0;
	BOOL InsideQuotes = FALSE;
	BOOL InsideDoubleQuotes = FALSE;
	BOOL removed = FALSE;
	for (int i = 0; i  < len + 1; i++) {
		if(text[i] == '"' && !InsideDoubleQuotes && !InsideQuotes) {
			InsideDoubleQuotes = TRUE;
			removed = TRUE;
		}
		else if (text[i] == '\'' && !InsideDoubleQuotes && !InsideQuotes) {
			InsideQuotes = TRUE;
			removed = TRUE;
		}
		else if (text[i] == '"' && InsideDoubleQuotes) {
			InsideDoubleQuotes = TRUE;
		}
		else if (text[i] == '\'' && InsideQuotes) {
			InsideQuotes = TRUE;
		}
		else {
			text[p] = text[i];
			p++;
		}
	}
	return removed;
	//text[p] = '\0';
}


Command* SepIntoCommand(char* command_str, Command* command, EnvVar* envVars) { //to choose if return a command obj or to pass one
	
	char command_str_copy[COMMAND_MAX_SIZE];
	snprintf(command_str_copy, COMMAND_MAX_SIZE, "%s", command_str);
	char* ptr = NULL;
	char* cmd;
	//int argc = 0;
	HANDLE outFile;
	HANDLE inFile;

	HANDLE hStdOutFile = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdInputFile = GetStdHandle(STD_INPUT_HANDLE);


	wchar_t* unicode_transfer;
	char* sep = " ";
	char* tok;
	Command* startCommand = command;
	command->argc = 0;


	/*char* new_command_string_buffer = PlaceEnvVars(command_str, envVars);
	snprintf(command_str, COMMAND_MAX_SIZE,"%s", new_command_string_buffer);
	free(new_command_string_buffer);

	if (command_str == NULL) {
		printf("OctoShell: Failed to load enviroment variables.\n");
		return NULL;
	}*/




	//seperate into tokens and 


	//add a redirect in
	tok = strtokCommand(command_str, sep);

	BOOL first = TRUE;
	while (tok != NULL && IsAssignment(tok)) {
		RemoveQuotes(tok);

		if (CheckVarName(tok)) {
			char* equal = strchr(tok, '=');
			if (!equal) return FALSE;
			int name_length = equal - tok;
			char* name = (char*)malloc(sizeof(char) * (name_length + 1));
			if (name == NULL) return FALSE;
			snprintf(name , name_length + 1,"%s", tok);
			char* value = (char*)malloc(strlen(equal)); //value length - 1 by check with the '=' sign.
			if (value == NULL) {
				free(name);
				return FALSE;
			}
			strcpy(value, equal + 1);
			if (!UpdateEnvVar(name, value, envVars)) {
				if (!AddEnvVar(name, value, envVars)) {
					free(name);
					free(value);
					return FALSE;
				}
			}
			first = FALSE;
			
		}
		else {
			printf("Varibale Name isn't valid, valid chars: 0-9 (must not be first), A-Z, a-z, _ .");
			return FALSE;
		}
		tok = strtokCommand(NULL, sep);


	}


	if (tok == NULL && !first) {
		return TRUE;
	}
	else if (!tok) return FALSE;
	
	//**********
	strcpy(command_str,(command_str_copy + (tok - command_str)));
	char* new_command_string_buffer = PlaceEnvVars(command_str, envVars);
	snprintf(command_str, COMMAND_MAX_SIZE, "%s", new_command_string_buffer);
	free(new_command_string_buffer);

	if (command_str == NULL) {
		printf("OctoShell: Failed to load enviroment variables.\n");
		return NULL;
	}

	tok = strtokCommand(command_str, sep);
	//******************
	cmd = tok;

	int name_length = (strlen(tok) + 1); // +5 for adding .exe
	command->name = (char*)malloc(sizeof(char) * name_length);

	//check malloc!!!!

	//
	/*if (!IsExe(cmd))
		snprintf(command->name, name_length, "%s.exe", cmd);
	else*/
	snprintf(command->name, name_length, "%s", cmd);

	//strcpy(command->name, cmd);
	tok = strtokCommand(NULL, sep);

	//create a command obj


	//parsing into the command.
	while (tok != NULL && command->argc < COMMAND_MAX_SIZE) {

		if (strncmp(tok, ">", COMMAND_MAX_SIZE) == 0) {
			tok = strtokCommand(NULL, sep);

			if (tok == NULL)
				break;

			RemoveQuotes(tok);


			if (SetRedirectOUT(tok, command) == NULL) return NULL;
			tok = strtokCommand(NULL, sep);

			continue;
		}
		
		//********************************************************************************** ->>>>>> REDIRECT INPUT NEED TO FIX!!!!
		//else if (strncmp(tok, "<", COMMAND_MAX_SIZE) == 0) {
		//	if (argc == 0) {
		//		if (SetRedirectINPUT(command->name, command) == NULL) return NULL;
		//		free(command->name);


		//		tok = strtokCommand(NULL, sep);
		//		if (tok == NULL)  return NULL; //add error handling
		//			
		//			command->name = (char*)malloc(sizeof(char) * (strlen(tok) + 1));
		//			strcpy(command->name, tok);
		//			tok = strtokCommand(NULL, sep);
		//			continue;

		//		
		//	}
		//	else {
		//		if (SetRedirectINPUT(command->argv[--argc], command) == NULL) return NULL;
		//		free(command->argv[argc]);
		//		tok = strtokCommand(NULL, sep);
		//		continue;
		//	}
		//}
		//***********************************************************************************


		if (strcmp(tok, "|") == 0) {
			tok = strtokCommand(NULL, sep);

			if (tok == NULL) continue;
			RemoveQuotes(tok);


			HANDLE hWrite, hRead;
			SECURITY_ATTRIBUTES sa = { 0 };
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = TRUE;
			sa.lpSecurityDescriptor = NULL;
			if (!CreatePipe(&hRead, &hWrite, &sa, 0))
			{
				printf("OctoShell: Create pipe failed....");
				return FALSE;

			}

			command->stdout_file = hWrite;
			command->redirect_out = TRUE;
			command->next = (Command*)malloc(sizeof(Command));
			if (!(command->next)) {
				printAllocationError();
				return FALSE;
			}
			command = command->next;
			Command_init(command, hStdInputFile, hStdOutFile);

			//set the command name:
			name_length = (strlen(tok) + 1); // + 5 for adding .exe
			command->name = (char*)malloc(sizeof(char) * name_length);// the size + null + .exe
			//check for succes fo malloc!!!!

			//
			/*if(!IsExe(tok))
				snprintf(command->name, name_length,"%s.exe", tok);
			else*/
			snprintf(command->name, name_length, "%s", tok);

			command->redirect_out = FALSE;
			command->argc = 0;
			command->stdin_file = hRead;
			command->redirect_in = TRUE;
			tok = strtokCommand(NULL, sep);

			continue;
		}
		command->argv[command->argc] = (char*)malloc(sizeof(char) * (strlen(tok) + 2));
		//check for succes fo malloc!!!!

			//
		strcpy(command->argv[command->argc], tok);
		command->argc++;
		tok = strtokCommand(NULL, sep);


	}

	command->next = NULL;

}

void ExitFree(Command* command) {
	FreeCommand(command);
	ZeroMemory(command, sizeof(Command));

}



BOOL AddString(char** src, char* dst) {
	char* p;
	p = realloc(*src, sizeof(char) * (strlen(*src) + strlen(dst) + 1));
	if (*src == NULL) {
		printAllocationError();
		return FALSE;
	}
	*src = p;
	if (strncat(*src, dst, strlen(dst))) {
		return FALSE;
	}
	return TRUE;
}

BOOL CheckIfFileExist(const char* path) {

	if (!HasExtension(path)) {
		char* formats[] = { ".exe", ".cmd", ".com" , ".bat" , ".ps1" };
		int pathLength = strlen(path) + 5;
		char* path_format = (char*)malloc(sizeof(char) * pathLength);
		if (path == NULL) {
			printAllocationError();
			return FALSE;
		}

		for (int i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
			snprintf(path_format, pathLength, "%s%s", path, formats[i]);
			if (PathFileExistsA(path_format)) {
				free(path_format);
				return TRUE;
			}
		}

		free(path_format);

		return FALSE;
	}
	else {
		return PathFileExistsA(path);
	}
}
BOOL HasExtension(const char* path) {
	int path_length = strlen(path);
	if (path < 5) return FALSE;
	const char* path_pointer = path + path_length - 4;
	char* formats[] = { ".exe", ".cmd", ".com" , ".bat" , ".ps1" };
	for (int i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
		if (strncmp(path_pointer, formats[i], 4) == 0) {
			return TRUE;
		}
	}
	return FALSE;

}

char* GetShellFilePath(int type) { //type: 0-> dir path, 1 -> dir + bin path;
	char* octoshell_bin_dir[MAX_PATH];
	char* path = NULL;
	size_t length;
	GetModuleFileNameA(NULL, octoshell_bin_dir, MAX_PATH);
	PathRemoveFileSpecA(octoshell_bin_dir);
	switch (type) {
	case 0:
		length = (strlen(octoshell_bin_dir) + 1);
		path = (char*)malloc(sizeof(char) * length);
		if (path == NULL) return NULL;
		snprintf(path, length, "%s", octoshell_bin_dir);
		break;
	case 1:
		length = (strlen(octoshell_bin_dir) + strlen("\\bin\\") + 1);
		path = (char*)malloc(sizeof(char) * length);
		if (path == NULL) return NULL;
		snprintf(path,length,"%s\\bin\\", octoshell_bin_dir);
		break;
	}
	return path;
}

BOOL LoadWindowsEnvVars(EnvVar* varList) {
	wchar_t* PWStr;
	wchar_t* content = NULL;
	wchar_t* env = GetEnvironmentStringsW();
	wchar_t* current = env;
	size_t skip_amount  = 0;
	while (*current) {
		skip_amount = wcslen(current) + 1;
		wchar_t* equal_sign = wcschr(current, L'=');
		if (equal_sign != NULL) {
			varList->nextVar = (EnvVar*)malloc(sizeof(EnvVar));
			if (!(varList->nextVar)) {
				FreeEnvironmentStrings(env);
				return  FALSE;
			}
			varList = varList->nextVar;
			PWStr = current;
			*equal_sign = L'\0';

			varList->name = utf16_to_utf8(PWStr);
			if (varList->name == NULL) {
				free(varList);
				return FALSE;
			}
			*equal_sign = L'=';

			PWStr = equal_sign + 1;
			varList->value = utf16_to_utf8(PWStr);
			if (varList->value == NULL) {
				free(varList->name);
				free(varList);

				return FALSE;
			}
		}
		current += skip_amount;

	}
	varList->nextVar = NULL;
	FreeEnvironmentStringsW(env);
	return TRUE;
}

//Add a new var but don't create a copy a the text but put the pointers as the value so be carefull when use.
BOOL AddEnvVar(char* name, char* value, EnvVar* envVars) {
	static EnvVar* last_var = NULL;
	if (last_var == NULL)
	{
		last_var = envVars;
		while (last_var->nextVar != NULL)
			last_var = last_var->nextVar;

		

		
	}

	last_var->nextVar = (EnvVar*)calloc(1, sizeof(EnvVar));
	if (last_var->nextVar == NULL) return FALSE;

	last_var = last_var->nextVar;
	last_var->name = name;
	last_var->value = value;
	return TRUE;
	
}

BOOL UpdateEnvVar(char* name, char* value, EnvVar* envVars) {
	EnvVar* current_var = envVars;
	

	while (current_var != NULL) {
		if (strcmp(current_var->name, name) == 0)
		{
			free(current_var->value);
			current_var->value = value;
			return TRUE;
		}
		current_var = current_var->nextVar;
	}
	return FALSE;

}

//GLOBAL VARIBALS-----------------------------------------------------------------------------------------------------------------------

BOOL(*func_arr[])(Command*) = { cd, pwd, echo,clear ,printLastEnvVar };
char* funcs_name[] = { "cd","pwd", "echo","clear", "last"};
char* funcs_name_cap[] = { "CD", "PWD","ECHO","CLEAR", "LAST"};
DirectoryNode* start_path;
DirectoryNode* path_pointer;
DirectoryNode* before;
EnvVar* envVars;
AliasVar* aliasVars;
char command_str[COMMAND_MAX_SIZE];
char* function_bin;  
//char* function_bin = "C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\x64\\Debug\\bin\\";
char* path;

//--------------------------------------------------------------------------------------------------------------------------------------


//function that uses the global varibals

void printLastEnvVar(Command* command) {
	EnvVar* pVar = envVars;
	while (pVar->nextVar != NULL)
		pVar = pVar->nextVar;

	printf("%s=%s\n", pVar->name, pVar->value);

}


//RETURN THE COMMAND WITH DIRETION TO THE FUNCTION IN HE BIN OF THE SHELL
Command* BinCommand_init(Command* command) {
	Command* copy_command = CopyCommand(command);

	if (copy_command == NULL)
		return NULL;
	size_t path_length = (strlen(function_bin) + strlen(copy_command->name) + 1 );
	char* bin_path = (char*)malloc(path_length * sizeof(char));
	if (bin_path == NULL) {
		printAllocationError();
		FreeCommand(copy_command);
		return NULL;
	}
	snprintf(bin_path, path_length,"%s%s", function_bin, copy_command->name); //functions folder path, command_name
	/*strcpy(bin_path, function_bin);
	strcat(bin_path, copy_command->name);*/

	free(copy_command->name);
	copy_command->name = bin_path;

	return copy_command;
}




int main()
{
	
	//GET THE PATH TO THE DIRECTORY OF THE FUNCTIONS
	function_bin = GetShellFilePath(1);


	//LOAD THE WINDOWS ENVIROEMTN AND SET THE VARS LIST
	envVars = (EnvVar*)calloc(1, sizeof(EnvVar));
	if (envVars == NULL) {
		printAllocationError();
		return 1;
	}

	char* OctoShellVarName = _strdup("OctoShell");
	if (OctoShellVarName == NULL) {
		free(envVars);
		printAllocationError();
		return 1;
	}

	envVars->name = OctoShellVarName;
	char* OctoShellVarValue = GetShellFilePath(0);
	if (OctoShellVarValue == NULL) {
		free(envVars->name);
		free(envVars);
		printAllocationError();
		return 1;
	}
	envVars->value = OctoShellVarValue;
	LoadWindowsEnvVars(envVars); 
	/*printEnvVars(envVars);

	char* test = "test/test/   $OctoShell";
	char* test2 = PlaceEnvVars(test, &envVars);*/




	//INITILIZE THE RESSOURCE ALLOCATOR THAT WILL BE USE TO MONITOR ALL THE PROCESSES THAT THE SHELL LOADED
	AllocResources procResources;
	procResources.si = NULL;
	procResources.pi = NULL;
	procResources.processes = NULL;
	procResources.processesNum = 0;
	
	//SET THE COLOR FOR THE UI DESIGN
	enable_ansi_colors();
	
	//SetThemeColors();

	//PRINT THE STARTING MESSAGE OF THE SHELL
	printOctopus();

	wprintf(L"Welcome to OctoShell 🐙くコ:彡\n");
	UINT original_cp = GetConsoleOutputCP(); // Save original code page
	SetConsoleOutputCP(CP_UTF8);
	printf("%u \n", original_cp);

	//GET THE STDIN AND STRDOUT HANDLES
	HANDLE hStdOutFile = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdInputFile = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdOutFile == INVALID_HANDLE_VALUE || hStdOutFile == NULL ||
		hStdInputFile == INVALID_HANDLE_VALUE || hStdInputFile == NULL) {
		DWORD e1 = GetLastError();
		printf("hOut=%p lastErr1=%lu \n", hStdOutFile, e1);
		printf("couldn't load stdin and stdout handles\n.");
		return 1;
	}

	//INTILIZE VARIBALES
	BOOL func_match_flag[COMMAND_MAX_SIZE];

	Command command = {0};
	Command* cmd_pointer;
	cmd_pointer = &command;
	int len;
	start_path = (DirectoryNode*)malloc(sizeof(DirectoryNode));
	start_path->name = (char*)malloc(sizeof(char) * 3);
	strcpy(start_path->name, "C:");
	start_path->next = NULL;
	path_pointer = start_path;


	const wchar_t* path_constW = L"C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\OctoShell\\tests_folder";
	const char* path_const = "C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\OctoShell\\tests_folder";

	//set the default path: 

	SetCurrentDirectoryW(path_constW);
	change_dir_Node(path_const); //NEED TO BE OUT OF COMMNET WHEN FINISHED.
	path = CreatePath(start_path);
	//************************************

	//THE MAIN LOOP THAT RUNS THE COMMANDS
	printf(ESC "[94m%s:~" ESC_FORGROUND_END "$ " ESC "[38;2;248;248;242m", path);
	while (fgets(command_str, COMMAND_MAX_SIZE, stdin)) {
		int commandIndex = 0;

		//Parsing the command into the Command structure.
		Command_init(&command, hStdInputFile, hStdOutFile);

		len = strlen(command_str);
		if (len > 0 && command_str[len - 1] == '\n')
			command_str[len - 1] = L'\0';

		//Remove the additional char fromthe command
		if (strcmp(command_str, "Exit()") == 0 || strcmp(command_str, "exit") == 0 || strcmp(command_str, "EXIT") == 0 || strcmp(command_str, "exit") == 0)
			break;

		//LOAD THE COMMAND STURCTURE BY THE COMMAND LINE THAT WAS PASED USING fgets()
		if (SepIntoCommand(command_str, &command, envVars) == NULL) {
			printf("Invalid Command\n");
			printf(ESC "[94m%s:~" ESC_FORGROUND_END "$ " ESC "[38;2;248;248;242m", path);
			ExitFree(&command);
			continue;
		}

		//CHCK IF TERE A COMMAND TO RUN
		if (command.name != NULL) {
			//call the function according to the command
			for (Command* pCur = &command; pCur != NULL; pCur = pCur->next) {
				func_match_flag[commandIndex] = FALSE;
				size_t command_length = strlen(pCur->name);
				for (int i = 0; i < sizeof(funcs_name) / sizeof(funcs_name[0]); i++) {
					if (strcmp(funcs_name[i], pCur->name) == 0 || strcmp(funcs_name_cap[i], pCur->name) == 0) {
						//command.argv = &(command.argv[1]);

						if (func_arr[i](pCur) == FALSE)
						{
							printf("There was a problem in function process.\n");
						};
						func_match_flag[commandIndex] = TRUE;
						pCur->built_in = TRUE;
						break;
					}
				}
				commandIndex++;
			}
			commandIndex = 0;
			BOOL skip = FALSE;
			for (Command* pCur = &command; pCur != NULL; pCur = pCur->next) {
				if (!func_match_flag[commandIndex] && !pCur->built_in) {
					Command* binCommand = BinCommand_init(pCur);

					if (binCommand == NULL)
					{

						ExitFree(pCur);
						skip = TRUE;
						break;
					}
					if (CheckIfFileExist(binCommand->name)) {
						if (Open_procces(binCommand, &procResources)) {
							if (pCur->redirect_in) pCur->redirect_in = FALSE;
							if (pCur->redirect_out) pCur->redirect_out = FALSE;
						}
						else {
							printf("OctoSell: failed to execute command.\n");

						}

					}
					else if (!Open_procces(pCur, &procResources)) { //CheckIfFileExist(pCur->name)

						char* formats[] = { ".cmd", ".com" , ".bat" , ".ps1" };
						BOOL found = FALSE;

						if (!HasExtension(pCur->name)) {

							char* originalName = pCur->name;
							int command_file_length = strlen(pCur->name) + 5; //+5 for inluding extension.
							char* path_with_format = (char*)malloc(command_file_length);
							pCur->name = path_with_format; //Change the name temporery
							for (int i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
								snprintf(path_with_format, command_file_length,"%s%s", originalName, formats[i]);
								if (Open_procces(pCur, &procResources)) {
									found = TRUE;
									break;
								}

							}
							pCur->name = originalName;
							free(path_with_format);
						}
						if(!found)
							printf("No execution file has been found, DON'T PLAY WITH US!!!\n");


						
						//if(!Open_procces(pCur, &procResources))
							//printf("Creating process failed.\n");
						
					}
					/*else {
						printf("No execution file has been found, DON'T PLAY WITH US!!!\n");
					}*/
					free(binCommand->name);
					free(binCommand);
				}
				commandIndex++;
			}

			//End the proccess that was lunched.

			EndProcesses(&procResources);
			if (skip) continue;
		}



		//free the memory that the command structure used
		
		ExitFree(&command);



		printf(ESC "[94m%s:~" ESC_FORGROUND_END "$ " ESC "[38;2;248;248;242m", path);

	}

	

	FreeEnvVars(envVars);
	freePathNode(start_path);
	EndThemeColors();
	printf("success");

	return 0;

}