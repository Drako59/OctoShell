#include "includes.h"

BOOL Alias(Command* command) {


}

char* GetAlias(AliasVar* Aliases, char* name) {
	while (Aliases) {
		if (strcmp(Aliases->name, name) == 0) {
			return _strdup(Aliases->value);
		}
		Aliases = Aliases->nextVar;
	}
	return NULL;
}

char* PlaceAliases(char* arg_par, AliasVar* aliases) {
	char* arg = _strdup(arg_par);
	if (arg == NULL) return NULL;
	char sep = ' ';
	int total_len = strlen(arg_par);

	char* placed_arg = malloc(sizeof(char) * total_len);
	char* placed_arg_alloc = NULL;


	char* value = NULL;
	int tok_len;
	BOOL first = TRUE;
	BOOL first_tok = TRUE;

	char* tok = strtokCommand(arg," ");
	while (tok != NULL) {
		
		
		
		tok_len = strlen(tok);
		if (first_tok) {
			value = GetAlias(aliases, tok);
			first_tok = FALSE;
		}
		else
			value = NULL;

		/*if (value == NULL) {
			if (placed_arg != NULL) free(placed_arg);
			return NULL;
		}*/


		total_len +=  value == NULL? tok_len : strlen(value) - tok_len;
		
		if (value != NULL) {
			placed_arg_alloc = (char*)realloc(placed_arg, sizeof(char) * (total_len + 1));
			if (placed_arg_alloc == NULL) {
				if (placed_arg != NULL) free(placed_arg);
				free(value);
				return NULL;
			}
			placed_arg = placed_arg_alloc;
		}
		if (first) {
			
			strcpy(placed_arg, value);
			first = FALSE;
		}
		else
		{
			strcat(placed_arg, value);
		}

		
		if (value != NULL)
			free(value);

		if (strcmp(tok, "|") == 0) {
			first_tok = TRUE;
		}

		tok = strtok(NULL, sep);

		if (tok != NULL)
			strcat(placed_arg, " ");

	}
	free(arg);
	return placed_arg;  //retun an allocation to heap so rememver to free.
}


