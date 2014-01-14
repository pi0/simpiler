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

#ifndef _cgenerator_included
#define _cgenerator_included

#include <stdio.h>

#include "cvar.h"
#include "ctoken.h"
#include "cglobal.h"

typedef struct {
char* Items[10000];
int count;
} cgenerator;

typedef enum { g_if_start,g_if_else,g_if_end } g_if_status;

typedef enum { g_while_start,g_while_end,g_while_condition } g_while_status;

typedef enum { g_main_start,g_main_end} g_program_status;


void g_init(cgenerator* g);

void g_append(cgenerator* g, char* data);

void g_comment(cgenerator* g, char* msg);

void g_main(cgenerator* g,g_program_status status);

void g_operator(cgenerator* g,  cvar*dst,cvar* left,cvar* right,cOperator op,char* hash);

void g_if(cgenerator* g,char* hash,g_if_status status);

void g_while(cgenerator* g,char* hash,g_if_status status);

void g_io(cgenerator* g,cvar* var,cKeyword type);

void g_vars(cgenerator*g , cvar_list* vars);

void g_dump(cgenerator *g,FILE* stream);


#endif
