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

#ifndef _clog_included
#define _clog_included

#include <stdio.h>

#include "ctoken.h"

typedef struct cToken cToken;//Definded as include dependancy with ctoken

typedef struct clog clog;
struct clog{
    char* Items[10000];
    int ItemsCount;
    FILE* out_stream;
};

void LoggerInit(clog* logger);

void LogError ( clog* logger,cToken* at,char *msg);

#endif
