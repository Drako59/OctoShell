#include "includes.h"

//GLOBAL ALIASES and VARS chains
EnvVar* envVars;
AliasVar* aliasVars;
//*********************

bool IsValidAliasName(const char* name)
{
	if (name == NULL || name[0] == '\0')
		return false;

	const char* invalid_chars = "/$`= \t\n|&;()<>'\"\\";

	for (const char* p = name; *p != '\0'; p++) {
		if (strchr(invalid_chars, *p) != NULL)
			return false;
	}

	return true;
}

BOOL AddAliases(char* name, char* value) {
	static AliasVar* last_var = NULL;
	if (last_var == NULL)
	{
		last_var = aliasVars;
		while (last_var->nextVar != NULL)
			last_var = last_var->nextVar;
	}

	last_var->nextVar = (AliasVar*)calloc(1, sizeof(AliasVar));
	if (last_var->nextVar == NULL) return FALSE;

	last_var = last_var->nextVar;
	last_var->name = name;
	last_var->value = value;
	return TRUE;

}

BOOL UpdateAliases(char* name, char* value) {
	AliasVar* current_var = aliasVars;


	while (current_var != NULL) {
		if (strcmp(current_var->name, name) == 0)
		{
			free(current_var->value);
			current_var->value = value;
			return TRUE;
		}
		current_var = current_var->nextVar;
	}

	return AddAliases(name, value);
}


BOOL Alias(Command* command) {
	char* sep = "=";
	for (int i = 0; i < command->argc; i++) {
		char* name = strtokCommand(command->argv[i], sep);
		
		if (!IsValidAliasName(name)) {
			continue;
		}

		name = _strdup(name);

		if (name == NULL) {
			printAllocationError();
			return FALSE;
		}
		char* value = strtokCommand(NULL,sep);

		if (value == NULL) {
			fputs("Alias: Invalid value was passed. for ", stderr);
			fputs(name, stderr);
			fputs("\n", stderr);


			return FALSE;
		}

		value = _strdup(value);

		if (value == NULL) {
			printAllocationError();
			free(value);
			return FALSE;
		}

		RemoveQuotes(value);

		UpdateAliases(name,value);

	}

	return TRUE;
}


void freeAlias(AliasVar* alias) {
	free(alias->name);
	free(alias->value);
	free(alias);
}


BOOL UnAlias(Command* command) {
	
	if (command->argc == 1 && (strcmp(command->argv[0], "-a") || strcmp(command->argv[0], "-A"))) {
		FreeAliasVars(aliasVars->nextVar);
		aliasVars->nextVar = NULL;
		return TRUE;
	}

	for (int i = 0; i < command->argc; i++) {
		char* name = command->argv[i];

		if (IsValidAliasName(name)) {

			AliasVar* pAlias = aliasVars;
			AliasVar* lastAlias = NULL;

			while (pAlias != NULL) {
				if (strcmp(name, pAlias->name) == 0) {
					if (lastAlias == NULL) {
						lastAlias = aliasVars;
						aliasVars = aliasVars->nextVar;
						freeAlias(lastAlias);
						return TRUE;
					}
					else {
						lastAlias = pAlias->nextVar;
						freeAlias(pAlias);
						return TRUE;
					}
				}
				lastAlias = pAlias;
				pAlias = pAlias->nextVar;
			}
		}
		else {
			fputs("UnAlias: Invalid alias name \"", stderr);
			fputs(name,stderr);
			fputs("\" was passed.\n", stderr);
			return FALSE;
		}
	}

	return FALSE;

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
	char* sep = " ";
	int total_len = strlen(arg_par);

	char* placed_arg = malloc(sizeof(char) * (total_len + 1));
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
			
			strcpy(placed_arg, value == NULL? tok : value);
			first = FALSE;
		}
		else
		{
			strcat(placed_arg, value == NULL ? tok : value);
		}

		
		if (value != NULL)
			free(value);

		if (strcmp(tok, "|") == 0) {
			first_tok = TRUE;
		}

		tok = strtokCommand(NULL, sep);

		if (tok != NULL)
			strcat(placed_arg, " ");

	}
	free(arg);
	return placed_arg;  //retun an allocation to heap so rememver to free.
}


void FreeAliasVars(AliasVar* envVars) {
	AliasVar* current;
	while (envVars != NULL) {
		free(envVars->name);
		free(envVars->value);
		current = envVars;
		envVars = envVars->nextVar;
		free(current);
	}

}
