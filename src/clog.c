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

#include <string.h>
#include <stdio.h>

#include "clog.h"

void LoggerInit(clog* logger)
{
    logger->ItemsCount=0;
    logger->out_stream=NULL;
}

void LogError ( clog* logger,cToken* at,char *msg)
{
  char m[100];
  sprintf(m,"error at line %d , col %d near %s \"%s\"   :   %s",at->Position.row+1,at->Position.col+1,
          GetMapKey (cTokenTypeToStringMap,at->Type),GetTokenValueString (at),msg);
  logger->Items[logger->ItemsCount++]=_(m);

  if(logger->out_stream!=NULL)
      fprintf(logger->out_stream,"%s\n",logger->Items[logger->ItemsCount-1]);

}
