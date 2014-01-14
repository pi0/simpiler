/*
 * This file is part of Simpiler.
 * Simpiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Simpiler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Simpiler.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _cvar_included
#define _cvar_included

#include <stdio.h>

#include "ctoken.h"
#include "clog.h"

typedef enum {
  cvar_int=1<<0,
  cvar_float=1<<1,
  cvar_string=1<<2,
  cvar_error=0
} cvarType;

typedef struct {
    cvarType Type;
    char* Name;
    struct cvar* next;
    union{ int i;float f;char c;char* s;} Value;
} cvar;

typedef struct {
  cvar* first_item ;
  cvar* last_item;
} cvar_list;

void cvarInit(cvar* v);

void cvarAddItem(cvar_list* list,cvar* item);

cvar* cvarGetItem(cvar_list* list,char* name);

cvarType getcTokenVarType ( cToken* token);

void ConvertLiteralsToVar(cTokenList* tokens,cvar_list* vars);

void  DumpVarList ( cvar_list* list, FILE* str);

cvar* GetVarByToken(cToken* token,cvar_list* vars,clog* logger);

#endif
