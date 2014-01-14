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
#include <stdio.h>

#include "cvar.h"
#include "ctoken.h"

void cvarInit(cvar* v)
{
    memset (v,0,sizeof(cvar));
}

void cvarAddItem(cvar_list* list,cvar* nitem)
{
    cvar* item= (cvar*)malloc(sizeof(cvar));
    *item=*nitem;
    item->next=NULL;

    if(list->first_item==NULL)
        list->first_item=list->last_item=item;
    else{
        list->last_item->next=item;
        list->last_item=item;
    }
}

cvar* cvarGetItem(cvar_list* list,char* name)
{
    cvar* i;
    for(i=list->first_item;i!=NULL;i=i->next)
        if(strcmp (name,i->Name)==0)
            return i;
    return NULL;
}

cvarType getcTokenVarType ( cToken* token)
{
    switch(token->data.i)
    {
    case ckeyword_int:   return cvar_int;
    case ckeyword_float:    return cvar_float;
    case ckeyword_string:   return cvar_string;
    default: return cvar_error;
    }
}

void ConvertLiteralsToVar(cTokenList* tokens,cvar_list* vars)
{
    cToken* curr;
    cvar v;
    cvarInit (&v);
    int i,convert;

    static const int ConstValues[][2] = {
        {ckeyword_null,0},
        {ckeyword_false,0},
        {ckeyword_true,1},
        {0,0}
    };


    for(curr=tokens->FirstItem;!cEot(curr);NextToken (&curr)){

        convert=0;

        if( curr->Type & ctoken_literal_val ){

            convert=1;

            switch(curr->Type){
            case ctoken_literal_int:
            case ctoken_literal_char:
                v.Value.i=curr->data.i;
                v.Type=cvar_int;
                break;
            case ctoken_literal_string:
                v.Value.s=curr->data.s;
                v.Type=cvar_string;
                break;

            }

        }
        else if (CheckTokenSeq (curr,NULL,"k",ckeyword_const_val))
        {
            for(i=0;ConstValues[i][0]!=0;i++)
                if(curr->data.i==ConstValues[i][0]){
                   v.Value.i=ConstValues[i][1];
                   v.Type=cvar_int;
                   convert=1;
                }
        }

        if(convert){

            v.Name=curr->Hash;
            cvarAddItem(vars,&v);

            curr->Type=ctoken_id;
            curr->data.s=_(v.Name);

            //Get ready for next var
            cvarInit (&v);
        }

    }//END FOR

}

void  DumpVarList ( cvar_list* list, FILE* str)
{
    cvar* v;
    for(v=list->first_item;v!=NULL;v=v->next)
        fprintf(str,"Variable %s\n",v->Name);
    printf("\n\n");
}

cvar* GetVarByToken(cToken* token,cvar_list* vars,clog* logger)
{
    cvar* v=NULL;
   if(token!=NULL) {
       v = cvarGetItem (vars,token->data.s);
       if(v==NULL)
           LogError (logger,token,"ID is not defined");
   }
   return v;
}
