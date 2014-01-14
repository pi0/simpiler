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

#ifndef _cparser_included
#define _cparser_included

#include "ctoken.h"
#include "cvar.h"
#include "cgenerator.h"

typedef struct cParseStatus {
  cToken* NextTokenToParse;
} cParseStatus;


void ParseTokens (cTokenList* Tokens,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseBlock(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseToken(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseDeclareToken(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseAssignmentToken(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseIF (cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseWhile(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseContidion(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseIO(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen);

cParseStatus ParseError(cToken* curr);

#endif
