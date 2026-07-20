#pragma once

#include "TypesDef.h"
#include "Parser.h"

#ifndef SHARED_H
#define SHARED_H

EnvVar* envVars; // The variable declaration
AliasVar* aliasVars;

#endif

char* PlaceAliases(char* arg_par, AliasVar* aliases);
void FreeAliasVars(AliasVar* envVars);
BOOL Alias(Command* command);
BOOL UnAlias(Command* command);