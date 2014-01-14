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

#ifndef _ctoken_included
#define _ctoken_included

#include <stdio.h>

#include "cglobal.h"
#include "clog.h"

extern int2string_map_item cTokenTypeToStringMap[];

typedef struct clog clog;

typedef enum  {
    ctoken_symbol=1<<0,
    ctoken_operator=1<<1,
    ctoken_keyword=1<<2,
    ctoken_id=1<<3,

    ctoken_literal_int =1<<4,
    ctoken_literal_float=1<<5,
    ctoken_literal_string=1<<6,
    ctoken_literal_char=1<<8,

    ctoken_literal_val = ctoken_literal_int | ctoken_literal_char |
    ctoken_literal_float | ctoken_literal_string,

    ctoken_eot=1<<7,
    ctoken_error=0
} cTokenType ;

typedef enum { cChar_empty,cChar_symbol,cChar_new_line,cChar_text } cCharType;

typedef enum { num_invalid,num_int,num_float } num_validate_status;


typedef struct {
    int row;
    int col;
    int offset;
} cTokenPosition;


typedef struct cToken cToken;
 struct cToken{
    cTokenType Type;
    cTokenPosition Position;
    char* Hash;
    struct cToken* NextToken;
    union {double d;float f;int i;char c;char *s;} data;
};

typedef struct {
    cToken* FirstItem;
    cToken* LastItem;
} cTokenList;


cTokenList GetToken (char* code);

cCharType GetCharType (char c);

cTokenType DetectTokenType(char* code,int start,int end);

int AddToken (cTokenList* list,char* code,int start,int end,cTokenPosition pos,cTokenType type);

void DetectOperators(cTokenList* tokens);

void DetectNumberSigns(cTokenList* tokens);

void DumpTokenList(cTokenList list,FILE* out_stream);

char* GetTokenValueString(cToken* i);

cToken* NextToken(cToken**token);

cToken* NextnToken(cToken** token,int i);

cToken* FindNextToken(cToken* curr,char format,int val);

cToken* FindTokenPair(cToken* curr,char o,char c);

cToken* CheckTokenSeq(cToken* current,clog* logger,char* format,...);

int CheckToken(cToken* current,char format,int val);

char* GetTokenHash(cToken* token);

num_validate_status is_valid_number(char* str,int s,int e);

int is_valid_id(char*str,int s,int e);

int is_digit(char c);

int is_wordchar(char c);

int cEot(cToken* t);

#endif

