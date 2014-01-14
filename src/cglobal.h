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

#ifndef _cglobal_included
#define _cglobal_included

extern const char cSymbols[];
extern const char cEmptyChars[];


enum cKeyword {

    ckeyword_if =1<<0,
    ckeyword_else=1<<1,
    ckeyword_while=1<<2,
    ckeyword_return=1<<3,

    ckeyword_int    =1<<4,
    ckeyword_float  =1<<5,
    ckeyword_string =1<<6,

    ckeyword_datatype= ckeyword_int|ckeyword_float|ckeyword_string,

    ckeyword_null   =1<<7,
    ckeyword_true   =1<<8,
    ckeyword_false  =1<<9,

    ckeyword_const_val= ckeyword_null | ckeyword_true | ckeyword_false,

    ckeyword_main = 1<<10,

    ckeyword_min = 1<<11,
    ckeyword_max = 1<<12,

    ckeyword_prints = 1<<13,
    ckeyword_printi = 1<<14,
    ckeyword_scani = 1<<15,

    ckeyword_io = ckeyword_printi | ckeyword_prints | ckeyword_scani,

    ckeyword_error =0
};
typedef enum cKeyword cKeyword;


typedef enum {

    coperator_equals=1<<0,
    coperator_notequal =1<<1,
    coperator_less=1<<2,
    coperator_lessequal =1<<3,
    coperator_greater=1<<4,
    coperator_greaterqual =1<<5,

    coperatortype_conditional=
    coperator_equals|coperator_notequal|coperator_less|
    coperator_lessequal|coperator_greater|coperator_greaterqual,

    coperator_assign= 1<<6,

    coperator_add      =1<<7,
    coperator_sub     =1<<8,
    coperator_multiply  =1<<9,
    coperator_divide    =1<<10,
    coperator_modolus   =1<<11,
    coperator_min=1<<12,
    coperator_max=1<<13,

    coperatortype_arithmetic=
    coperator_add|coperator_sub|coperator_multiply|
    coperator_divide|coperator_modolus|
    coperator_min|coperator_max ,


    coperator_and =1<<14,
    coperator_or =1<<15,
    coperator_not =1<<16,

    coperatortype_bitwise=
    coperator_and|coperator_or|coperator_not,

    coperator_logical_and=1<<17,
    coperator_logical_or =1<<18,
    coperator_logical_not=1<<19,

    coperatortype_logical= coperator_logical_not |coperator_logical_and|coperator_logical_or,

    coperator_all = coperatortype_conditional | coperatortype_bitwise |
    coperatortype_logical | coperatortype_arithmetic ,

    coperator_error =0
} cOperator;


typedef struct{
    const char* key;
    int    value;
} int2string_map_item;

extern const int2string_map_item cKeywordToStringMap[];
extern const int2string_map_item cOperatorToStringMap[];

int GetMapValue (int2string_map_item *map,const char* key);

const char* GetMapKey (int2string_map_item *map,int value);

char* _(char*);

char* __(char* format,...);

char* itoa(int num);


#endif
