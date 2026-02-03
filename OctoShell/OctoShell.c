// OctoShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "basicImports.h"

#include "includes.h"

#define ESC "\x1b"

//typedef struct DirectoryNode {
//	const wchar_t* name;
//	struct DirectoryNode* next;
//
//} DirectoryNode;

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

void printAllocationError(void) {
	printf("OctoShell: There is no available space in heap. try to upgrade your pc maybe?(pls?) :(\n");

}

void FreeCommand(Command* cmd_pointer) {
	while (cmd_pointer != NULL) {
		free(cmd_pointer->name);
		if (cmd_pointer->redirect_in)
			CloseHandle(cmd_pointer->stdin_file);
		if (cmd_pointer->redirect_out)
			CloseHandle(cmd_pointer->stdout_file);
		for (int i = 0; i < cmd_pointer->argc; i++) {
			free(cmd_pointer->argv[i]);
		}
		cmd_pointer = cmd_pointer->next;
	}
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
	//printf("Here");

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




char* strtokCommand(char* command_str, char* sep) {
	

	BOOL insideQuote = FALSE;
	BOOL insideDoubleQuote = FALSE;
	static char* command = NULL;
	if (command_str != NULL)
		command = command_str;
	char* copyCommand;
	if (command != NULL) {
		for (int i = 0; i < strlen(command); i++) {
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
				command = &(command[i + 1]);
				return copyCommand;
			}
		}

		if (insideDoubleQuote || insideQuote) {
			printf("OctoShell: There is must be a matching quote in a parsed string.");
			command = NULL;
			return NULL;
		}

		copyCommand = command;
		command = NULL;
		return copyCommand;
	}
	return NULL;


}

Command* SepIntoCommand(char* command_str, Command* command) { //to choose if return a command obj or to pass one
	char* ptr = NULL;
	char* cmd;
	int argc = 0;
	HANDLE outFile;
	HANDLE inFile;

	wchar_t* unicode_transfer;
	char* sep = " ";
	char* tok;
	//seperate into tokens and 
	tok = strtokCommand(command_str, sep);
	if (!tok) return NULL;
	cmd = tok;
	command->name = (char*)malloc(sizeof(char) * (strlen(tok) + 1));
	strcpy(command->name, cmd);
	//printf("commandName->%s", command->name); //DEBUG
	tok = strtokCommand(NULL, sep);
	//create a command obj


	//parsing into the command.
	while (tok != NULL && command->argc < COMMAND_MAX_SIZE) {
		if (strncmp(tok, ">", COMMAND_MAX_SIZE) == 0) {
			tok = strtokCommand(NULL, sep);
			if (tok == NULL)
				break;
			if (SetRedirectOUT(tok, command) == NULL) return NULL;
			tok = strtokCommand(NULL, sep);
			continue;
		}
		command->argv[argc] = (char*)malloc(sizeof(char) * (strlen(tok) + 2));
		//printf("argc=%d, tok=\"%ls\", dst=%p\n", argc, tok, command->argv[argc]); //DEBUG
		//printf("tok->%s", tok); //DEBUG
		strcpy(command->argv[argc], tok);
		argc++;
		tok = strtokCommand(NULL, sep);

	}

	command->argc = argc;
	command->next = NULL;


}

void ExitFree(Command* command) {
	FreeCommand(command);

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



//GLOBAL VARIBALS-----------------------------------------------------------------------------------------------------------------------




BOOL(*func_arr[])(Command*) = { cd, pwd, echo,clear };
char* funcs_name[] = { "cd","pwd", "echo","clear" };
char* funcs_name_cap[] = { "CD", "PWD","ECHO","CLEAR" };
DirectoryNode* start_path;

DirectoryNode* path_pointer;
DirectoryNode* before;
char command_str[COMMAND_MAX_SIZE];
char* function_bin = "C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\x64\\Func_Bin\\";
char* path;
//--------------------------------------------------------------------------------------------------------------------------------------
//function that uses the global varibals

Command* BinCommand(Command* command) {
	Command* copy_command = CopyCommand(command);

	if (copy_command == NULL)
		return NULL;

	char* bin_path = (char*)malloc((strlen(function_bin) + strlen(copy_command->name) + 1) * sizeof(char));
	if (bin_path == NULL) {
		printAllocationError();
		FreeCommand(copy_command);
		return NULL;
	}
	strcpy(bin_path, function_bin);
	strcat(bin_path, copy_command->name);
	//printf("%s\n", copy_command->name); //DEBUG

	//printf("%s\n", bin_path); //DEBUG
	free(copy_command->name);
	copy_command->name = bin_path;

	return copy_command;
}


void Command_init(Command* command, HANDLE hStdInputFile, HANDLE hStdOutFile) {
	command->stdin_file = hStdInputFile;
	command->stdout_file = hStdOutFile;
	command->redirect_in = FALSE;
	command->redirect_out = FALSE;
}

int main()
{
	//printf("%d\n", sizeof(char*));
	enable_ansi_colors();
	printOctopus();
	wprintf(L"Welcome to OctoShell 🐙\n");
	UINT original_cp = GetConsoleOutputCP(); // Save original code page

	printf("%u \n", original_cp);
	SetConsoleOutputCP(CP_UTF8);

	HANDLE hStdOutFile = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdInputFile = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdOutFile == INVALID_HANDLE_VALUE || hStdOutFile == NULL ||
		hStdInputFile == INVALID_HANDLE_VALUE || hStdInputFile == NULL) {
		DWORD e1 = GetLastError();
		printf("hOut=%p lastErr1=%lu \n", hStdOutFile, e1);
		printf("couldn't load stdin and stdout handles\n.");
		return 1;
	}
	BOOL func_match_flag = FALSE;

	Command command;
	Command* cmd_pointer;
	cmd_pointer = &command;
	int len;
	start_path = (DirectoryNode*)malloc(sizeof(DirectoryNode));
	start_path->name = (char*)malloc(sizeof(char) * 3);
	strcpy(start_path->name, "C:");
	start_path->next = NULL;
	path_pointer = start_path;


	//const wchar_t* path_const = path;
	//const wchar_t* path_const = L"C:\\Users\\User\\source\\repos\\Drako59\\OctoShell\\OctoShell";
	const wchar_t* path_constW = L"C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\OctoShell\\tests_folder";
	const char* path_const = "C:\\Users\\ayele\\source\\repos\\Drako59\\OctoShell\\OctoShell\\tests_folder";

	//set the default path: 

	SetCurrentDirectoryW(path_constW);
	change_dir_Node(path_const); //NEED TO BE OUT OF COMMNET WHEN FINISHED.
	path = CreatePath(start_path);
	//************************************

	//wprintf(L"<%s>", path);
	printf("%s:~$", path);
	while (fgets(command_str, COMMAND_MAX_SIZE, stdin)) {

		//Set the redirections
		Command_init(&command, hStdInputFile, hStdOutFile);
		func_match_flag = FALSE;

		len = strlen(command_str);
		if (len > 0 && command_str[len - 1] == '\n')
			command_str[len - 1] = L'\0';

		//Remove the additional char fromthe command
		if (strcmp(command_str, "Exit()") == 0 || strcmp(command_str, "exit") == 0 || strcmp(command_str, "EXIT") == 0 || strcmp(command_str, "exit") == 0)
			break;

		if (SepIntoCommand(command_str, &command) == NULL) {
			printf("Invalid Command\n");
			printf("%s:~$", path);
			continue;
		}

		//wprintf(L"command->Name: %s, command->argv: %s , command->argc: %d\n", command.name, command.argv[0],command.argc); //DEBUG
		//call the function according to the command
		for (int i = 0; i < sizeof(funcs_name) / sizeof(funcs_name[0]); i++) {
			if (strcmp(funcs_name[i], command.name) == 0 || strcmp(funcs_name_cap[i], command.name) == 0) {
				//command.argv = &(command.argv[1]);

				if (func_arr[i](&command) == FALSE)
				{
					printf("There was a problem in function process.\n");
				};
				func_match_flag = TRUE;
				break;
			}
		}

		if (!func_match_flag) {

			Command* binCommand = BinCommand(&command);
			//printf("%s\n", binCommand->name);
			//printf("BinCommand->argv[i]->%s", binCommand->argv[0]);

			if (binCommand == NULL)
			{
				ExitFree(&command);
				continue;
			}
			if (Open_procces(binCommand)) {
				printf("");
			}
			else if (!Open_procces(&command))
				printf("Creating process failed.\n");
			free(binCommand->name);
			free(binCommand);

		}




		//free the memory that the command structure used

		ExitFree(&command);



		printf("%s:~$", path);

	}


	freePathNode(start_path);

	printf("success");

	return 0;

}