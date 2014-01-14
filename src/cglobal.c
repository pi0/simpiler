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
#include <stdlib.h>
#include <stdarg.h>

#include "cglobal.h"

const int _c_max_id_size=100;
const char cSymbols[] = ";,:(){}[]<>!=+-*/|&%";
const char cEmptyChars[] = " \r\n\t";

const int2string_map_item cKeywordToStringMap[] =
{
    {"if",ckeyword_if},
    {"else",ckeyword_else},
    {"while",ckeyword_while},
    {"return",ckeyword_return},

    {"int",ckeyword_int},
    {"float",ckeyword_float},
    {"string",ckeyword_string},

    {"char",ckeyword_int},//Patched value!
    {"bool",ckeyword_int},//Patched value!
    {"void",ckeyword_int},//Patched value!!

    {"null",ckeyword_null},
    {"true",ckeyword_true},
    {"false",ckeyword_false},

    {"main",ckeyword_main},

    {"min",ckeyword_min},
    {"max",ckeyword_max},

    {"prints",ckeyword_prints},
    {"printi",ckeyword_printi},
    {"scani",ckeyword_scani},

    {"",-1}//DONT REMOVE ME!
};

const int2string_map_item cOperatorToStringMap[] =
{
    {"==",coperator_equals},
    {"!=",coperator_notequal},
    {"<",coperator_less},
    {"<=",coperator_lessequal},
    {">",coperator_greater},
    {">=",coperator_greaterqual},

    {"=",coperator_assign},

    {"+",coperator_add},
    {"-",coperator_sub},
    {"*",coperator_multiply},
    {"/",coperator_divide},
    {"%",coperator_modolus},

    {"&",coperator_and},
    {"|",coperator_or},
    {"!",coperator_not},

    {"&&",coperator_logical_and},
    {"||",coperator_logical_or},
    {"!",coperator_logical_not},

    {"",-1}//Dont Remove me!
};


int  GetMapValue (int2string_map_item *map,const char* key)
{
    int i;
    for(i=0;map[i].value!=-1;i++)
        if(strncmp (key,map[i].key,strlen(map[i].key))==0)
            return map[i].value;
    return -1;
}

const char* GetMapKey (int2string_map_item *map,int value)
{
    int i;

    for(i=0;map[i].value!=-1;i++)
        if(value==map[i].value)
            return map[i].key;

    return NULL;
}

char* _(char* s)
{
    char* m=malloc(strlen(s)+1);
    strcpy (m,s);
    return m;
}

char* __(char* format,...)
{
    va_list p;
    int p_c,i,j;
    char buffer[100];

    for(i=0,p_c=0;format[i]!='\0';i++)
        if(format[i]=='%')
            p_c++;
    va_start(p,p_c);


    for(i=0,j=0;format[i]!=0;i++)
    {
        if(format[i]!='%'){
            buffer[j++]=format[i];
        }else{
           char* t=va_arg(p,char*);
           int l=strlen(t);
           strcpy (buffer+j,t);
           j+=l;
        }
    }
    buffer[j]=0;

    va_end(p);

    return _(buffer);
}

char* itoa(int num)
{
    char buff[10];
    sprintf(buff,"%d",num);
    return _(buff);
}
