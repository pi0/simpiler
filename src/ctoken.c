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
#include <stdarg.h>

#include "ctoken.h"
#include "cglobal.h"
#include "clog.h"

int2string_map_item cTokenTypeToStringMap[]={
    {"ID",ctoken_id},
    {"operator",ctoken_operator},
    {"keyword",ctoken_keyword},
    {"symbol",ctoken_symbol},
    {"Integer",ctoken_literal_int},
    {"char",ctoken_literal_char},
    {"string",ctoken_literal_string},
    {"",-1}
};


cTokenList GetToken (char* code)
{
    cTokenList tokens;
    tokens.FirstItem=NULL;
    cTokenPosition curr_pos={0,0,0},
            last_pos={0,0,0};
    cCharType last_char_type=GetCharType (code[0]),
            next_char_type=last_char_type;

    int i;

    //Pass 1 : Get Keywords, Constants , Strings & symbols

    for (;code[curr_pos.offset]!='\0';curr_pos.offset++,curr_pos.col++){

        //Detect strings
        if(code[curr_pos.offset]=='"' ){
            for(i=curr_pos.offset+1.;code[i]!='"';i++){}
            AddToken (&tokens,code,curr_pos.offset+1,i-1,curr_pos,ctoken_literal_string);
            curr_pos.offset=last_pos.offset=i+1;
        }

        if(next_char_type==cChar_new_line){//next is now current
            curr_pos.row++;
            curr_pos.col=0;
        }

        next_char_type=GetCharType(code[curr_pos.offset+1]);

        if(last_char_type!=next_char_type||next_char_type==cChar_symbol)
        {

            if(last_char_type != cChar_new_line && last_char_type != cChar_empty)
                AddToken (&tokens,code,last_pos.offset,curr_pos.offset,last_pos,
                          DetectTokenType (code,last_pos.offset,curr_pos.offset));
            last_pos=curr_pos;
            last_pos.offset++;
            if(last_char_type!=cChar_new_line)
                last_pos.col++;
            else
                last_pos.col=0;

            last_char_type=next_char_type;
        }
    }

    //Pass 2 : Detect operators
    DetectOperators(&tokens);


    //Add EOT to end of list
    cToken* t=(cToken*)malloc(sizeof(cToken));
    memset (t,0,sizeof(t));//Invalidate everything
    t->Type=ctoken_eot;
    t->NextToken=t;//LOOP!
    tokens.LastItem->NextToken=t;

    return tokens;
}

cCharType GetCharType (char c)
{
    if(c=='\n')
        return cChar_new_line;

    if (strchr (cEmptyChars,c))
        return cChar_empty;

    if (strchr (cSymbols,c))
        return cChar_symbol;

    return cChar_text;
}

cTokenType DetectTokenType(char* code,int start,int end)
{
    cTokenType token_type;
    if(GetCharType (code[start]) == cChar_symbol)
        token_type=ctoken_symbol;
    else
        if(GetMapValue (cKeywordToStringMap,&code[start])!=-1)
            token_type=ctoken_keyword;
        else{
            switch(is_valid_number(code,start,end))
            {
            case num_int:
                token_type=ctoken_literal_int;
                break;
            case num_float:
                token_type=ctoken_literal_float;
                break;
            default:
                if(code[start]==code[end] && code[start]=='\'')
                    token_type=ctoken_literal_char;
                else
                    if(is_valid_id(code,start,end))
                        token_type=ctoken_id;//Ad as ID
                    else token_type=ctoken_error;
                break;
            }
        }
    return token_type;
}

int AddToken (cTokenList* list,char* code,int start,int end,cTokenPosition pos,cTokenType type)
{
    cToken* token=(cToken*)malloc(sizeof(cToken));
    int len;
    char tmp[10];

    token->Type=type;
    token->Position=pos;

    switch(type)
    {
    case ctoken_id:
        len=end-start+1;
        token->data.s=(char*)malloc(len+1);
        strncpy (token->data.s,&code[start],len);
        break;
    case ctoken_literal_float:
    case ctoken_literal_int:
        strncpy(tmp,&code[start],end-start+1);
        if(type==ctoken_literal_int)
            token->data.i=atoi (tmp);
        else
            token->data.f=atof (tmp);
        break;
    case ctoken_literal_char:
        token->data.i=code[start+1];
        break;
    case ctoken_literal_string:
        strncpy(tmp,&code[start],end-start+1);
        tmp[end-start+1]='\0';
        token->data.s=_(tmp);
        break;
    case ctoken_keyword:
        token->data.i=GetMapValue (cKeywordToStringMap,&code[start]);
        break;
    case ctoken_symbol:
        token->data.c=code[start];
        break;

    case ctoken_operator:break;//pass 1 won't add operators!

    case ctoken_error:
    default:
        token->Type=ctoken_error;
        token->data.s="Unable to parse token";//TODO : A better error?
        break;
    }

    if(list->FirstItem==NULL)
        list->FirstItem=list->LastItem=token;
    else{
        list->LastItem->NextToken=token;
        list->LastItem=token;
    }

    token->Hash=GetTokenHash (token);

    return 1;
}

void DetectOperators(cTokenList* tokens)
{
    cToken* t;
    char bff[3];
    cOperator operator_type;

    for(t=tokens->FirstItem;t!=NULL;t=t->NextToken){
        if(t->Type==ctoken_symbol){
            int extended=0;
            if(t->NextToken!=NULL &&
                    t->NextToken->Type==ctoken_symbol){
                sprintf(bff,"%c%c",t->data.c,t->NextToken->data.c);
                extended=1;
            }
            else
                sprintf(bff,"%c",t->data.c);

            int recheck=1;
            do{
                recheck=0;
                operator_type=GetMapValue(cOperatorToStringMap,bff);

                if(operator_type!=-1){
                    t->Type=ctoken_operator;
                    t->data.i=operator_type;
                    if(extended){
                        cToken* nxt=t->NextToken;
                        t->NextToken=t->NextToken->NextToken;
                        free(nxt);
                    }
                }else if(extended){
                    //Try to fall back!
                    bff[1]='\0';
                    extended=0;
                    recheck=1;
                }
            } while(recheck);

        }
    }
}


void DetectNumberSigns ( cTokenList* list)
{
    cToken *curr,*prv;

    for(curr=prv=list->FirstItem;curr && !cEot(curr);curr=curr->NextToken)
    {
        if(CheckTokenSeq (curr,NULL,"-"))
            if(curr->NextToken->Type & ctoken_literal_val) {
                curr->NextToken->data.i*=-1;
                prv->NextToken=curr->NextToken;
                curr=curr->NextToken;
            }

        prv=curr;
    }

}

void DumpTokenList(cTokenList list,FILE* out_stream)
{
    char *type_str,*value_str;
    cToken* i;

    for(i=list.FirstItem;!cEot(i);i=i->NextToken){
        type_str=GetMapKey (cTokenTypeToStringMap,i->Type);
        value_str=GetTokenValueString (i);

        fprintf(out_stream,"line:%4d , col:%3d:\t\t %-10.15s %s\n",
                i->Position.row+1,i->Position.col+1,type_str,value_str);
    }
    printf("\n\n");
}

char* GetTokenValueString(cToken* i)
{
    char value_str[100]="";
    switch(i->Type) {
    case ctoken_id:
        sprintf(value_str,"%s",i->data.s);
        break;
    case ctoken_literal_int:
    case ctoken_literal_char:
        sprintf(value_str,"%d",i->data.i);
        break;
    case ctoken_literal_float:
        sprintf(value_str,"%f",i->data.f);
        break;
    case ctoken_keyword:
        sprintf(value_str,"%s",GetMapKey (cKeywordToStringMap,i->data.i));
        break;
    case ctoken_operator:
        sprintf(value_str,"%s",GetMapKey (cOperatorToStringMap,i->data.i));
        break;
    case ctoken_symbol:
        sprintf(value_str,"%c",i->data.c);
        break;
    case ctoken_literal_string:
        sprintf(value_str,"\"%s\"",i->data.s);
        break;
    }
    return _(value_str);
}

cToken* NextToken(cToken** token)
{
    if(!cEot(*token)){
        if((*token)->NextToken!=NULL)
            *token=(*token)->NextToken;
        return *token;
    }
    if((*token)->Type==ctoken_eot)
        return *token;
    else
        return NULL;
}

cToken* NextnToken(cToken** token,int i)
{
    if(i<1)return NULL;
    i--;
    while(i--)
        NextToken(token);
    return NextToken(token);
}

cToken* FindNextToken(cToken* curr,char format,int val){
    while(!cEot(curr)){
        NextToken (&curr);
        if(CheckToken (curr,format,val))
            break;
    }
    return curr;
}

cToken* FindTokenPair(cToken* curr,char o,char c)
{
    int status=1;
    while(NextToken (&curr)){

        if(cEot(curr))
            return curr;

        if(CheckToken (curr,o,o))
            status++;
        else if (CheckToken(curr,c,c))
            status--;
        if(status==0)
            return curr;
    }

    return NULL;
}

cToken* CheckTokenSeq(cToken* current,clog* logger,char* format,...)
{
    // (k)eyword   (i)d   (o)perator
    //.... or any other char for symbols
    int i,j,val,error=0;
    char* t;

    va_list arg_params;

    int len=0;
    for(i=0;format[i]!='\0';i++)
        if(strchr ("ko",format[i]))
            len++;
    va_start(arg_params,len);


    for(i=0;format[i]!='\0';i++,NextToken (&current)) {

        val=format[i];

        if(strchr("ko",format[i]))
            val=va_arg(arg_params,int);

        if( ! CheckToken (current,format[i],val)){
            error=1;
            if(logger){
                char buffer[100];
                switch(format[i])
                {
                case 'k' :
                    t=GetMapKey (cKeywordToStringMap,val);
                    if(t==NULL)
                        t="\b";
                    sprintf(buffer,"Keyword %s expected",t);
                    break;
                case 'o':
                    t=GetMapKey (cOperatorToStringMap,val);
                    if(t==NULL)
                        t="\b";
                    sprintf(buffer,"Operator %s expected",t);
                    break;
                case 'i' :
                    sprintf(buffer,"ID expected");
                    break;
                default:
                    sprintf(buffer,"expected \"%c\" ",format[i]);
                }

                LogError (logger,current,buffer);
            }
            else break;
        }
    }
    va_end(arg_params);

    if(!error)
        return current;
    else return NULL;

}

int CheckToken(cToken* current,char format,int val)
{
    int i,correct;

    static const int Filter[][2] =
    { {'k',ctoken_keyword},{'o',ctoken_operator} ,{'\0',0}};

    if(format=='i')
        correct=current->Type==ctoken_id;
    else
        correct = (current->Type==ctoken_symbol &&
                   (val==current->data.c));

    for(i=0;Filter[i][0]!='\0';i++)
        if(Filter[i][0]==format){
            correct = (current->Type==Filter[i][1] &&
                    (val&current->data.i));
            break;
        }

    return correct;
}

char* GetTokenHash(cToken* token)
{
    char buff[10];
    int hash_val=(token->Position.offset)*10;
    int i;
    buff[0]='x';
    for(i=1;i<8 && hash_val>0;i++,hash_val/=10)
        buff[i]='a'+hash_val%25;
    buff[i]='\0';
    return _(buff);
}

num_validate_status is_valid_number(char* str,int s,int e)
{
    int dot_count=0;
    int i;
    for(i=s;i<=e;i++)
        if ( str[i]<'0' || str[i]>'9'){
            if(str[i]=='.'){
                dot_count++;
                if(dot_count>1)
                    return num_invalid;
            }else return num_invalid;
        }
    return dot_count>0?num_float:num_int;
}

int is_valid_id(char*str,int s,int e)
{
    int i=s;

    if(!is_wordchar (str[i++]))
        return 0;

    for(;i<=e;i++)
        if(!is_digit (str[i]) && !is_wordchar (str[i]))
            return 0;

    return 1;
}

int is_digit(char c)
{
    return (c>='0' && c<='9');
}

int is_wordchar(char c)
{
    return  ( (c>='a' && c<='z') || (c>='A' && c<='Z') ) || c=='_';
}

int cEot(cToken* t){
    if(t==NULL)return 1;
    return t->Type==ctoken_eot;
}
