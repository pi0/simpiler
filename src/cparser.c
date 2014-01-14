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
 * along with Sampiler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cparser.h"
#include "ctoken.h"
#include "clog.h"
#include "cvar.h"
#include "cgenerator.h"

void ParseTokens (cTokenList* Tokens,clog* logger,cvar_list* vars,cgenerator* gen)
{
    cToken* curr=Tokens->FirstItem;

    if(!CheckTokenSeq (curr,logger,"kk(){",ckeyword_datatype,ckeyword_main)) {
        LogError (logger,curr,"unable to find application main entry point");
        return;
    }

    NextnToken (&curr,5);//skip to first statement

    g_main (gen,g_main_start);

    ConvertLiteralsToVar(Tokens,vars);

    curr=ParseBlock(curr,logger,vars,gen).NextTokenToParse;//Parse entire main block
    if(!cEot(curr))
        LogError (logger,curr,"No operations allowed outside main");

    g_main (gen,g_main_end);

    g_vars(gen,vars);
}


cParseStatus ParseBlock(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    cToken* end=FindTokenPair (curr,'{','}');

    if(end==NULL){
        LogError (logger,curr,"cannot find end of block . symbol \"{\" expected.");
        return ParseError(curr);
    }

    while(!cEot(curr) && curr!=end && curr->Position.offset  <  end->Position.offset)
        curr=ParseToken(curr,logger,vars,gen).NextTokenToParse;

    NextToken (&curr);//skip "{"

    cParseStatus s;
    s.NextTokenToParse=curr;
    return s;
}

cParseStatus ParseToken(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    switch(curr->Type){

    case ctoken_keyword:

        if(CheckTokenSeq (curr,NULL,"k",ckeyword_datatype))
            return ParseDeclareToken (curr,logger,vars,gen);

        else if (CheckTokenSeq (curr,NULL,"k",ckeyword_io))
            return ParseIO(curr,logger,vars,gen);

        else switch (curr->data.i) {

        case ckeyword_if:
            return ParseIF (curr,logger,vars,gen);

        case ckeyword_while:
            return ParseWhile(curr,logger,vars,gen);

        default:
            LogError (logger,curr,"unexpected keyword");
            return ParseError (curr);
        }

        break;//END keyword case

    case ctoken_id:
        return ParseAssignmentToken (curr,logger,vars,gen);

    case ctoken_symbol:
        switch(curr->data.c)
        {
        case '{' :
            NextToken (&curr);//skip "{"
            return ParseBlock(curr,logger,vars,gen)  ;

        case ';' :
            NextToken (&curr);//skip ";"
            return ParseToken(curr,logger,vars,gen);

        default:
            LogError (logger,curr,"unexpected symbol");
            return ParseError(curr);
        }
    }

    LogError (logger,curr,"unexpected token");
    return ParseError (curr);
}

cParseStatus ParseDeclareToken(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    cToken* ID;
    cvar new_var;
    cvarInit(&new_var);

    if(!(new_var.Type=getcTokenVarType(curr))) {
        LogError (logger,curr,"unknown data type");
        return ParseError(curr);
    }

    NextToken (&curr); //skip datatype eg : int

ParseDeclareToken_GetID:

    if(!CheckTokenSeq (curr,logger,"i"))
        return ParseError(curr);
    ID=curr;//Keep it for next steps

    new_var.Name=_(curr->data.s);

    if (cvarGetItem (vars,new_var.Name))
    {
        LogError (logger,curr,"ID is already defined");
        return ParseError(curr);
    }

    cvarAddItem(vars,&new_var);

    if(CheckTokenSeq (curr->NextToken,NULL,"o",coperator_assign))
        return ParseAssignmentToken (ID,logger,vars,gen);

    NextToken (&curr);//Skip ID

    if(CheckTokenSeq (curr,NULL,",")) {
        NextToken (&curr);
        goto  ParseDeclareToken_GetID;
    }

    if( !CheckTokenSeq (curr,logger,";") ){
        return ParseError(curr);
    }

    NextToken (&curr); //skip ";"

    cParseStatus s;
    s.NextTokenToParse=curr;
    return s;
}

cParseStatus ParseAssignmentToken(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    cvar *var,*lft_var=NULL,*rgt_var=NULL;
    cToken* lft_token=NULL,*rgt_token=NULL;
    cOperator opr;

    if( !(var=cvarGetItem (vars,curr->data.s)) ) {
        LogError (logger,curr,"ID is not defined");
        return ParseError (curr);
    }

    NextToken (&curr);//skip ID name

    if(!CheckTokenSeq (curr,logger,"o",coperator_assign))
        return ParseError (curr);
    char* hash=curr->Hash;

    NextToken (&curr);//skip "="


    if(CheckTokenSeq (curr,NULL,"i")){

        lft_token=curr;

        NextToken (&curr);//skip left token

        if(CheckTokenSeq (curr,NULL,";")){
            opr=coperator_add;

        }else if(CheckTokenSeq (curr,logger,"oi;",coperator_all)){
            opr= (cOperator)curr->data.i;
            NextToken (&curr);//skip operator
            rgt_token=curr;
            NextToken (&curr);//skip right token
        }

        else return ParseError(curr);
    }
    else if (CheckTokenSeq (curr,NULL,"k",ckeyword_min  | ckeyword_max)) {

        opr=(CheckTokenSeq (curr,NULL,"k",ckeyword_min))?coperator_min:coperator_max;

        NextToken (&curr);//skip min,max

        if(!CheckTokenSeq (curr,logger,"(i,i);"))
            return ParseError (curr);

        NextToken (&curr);//skip "("
        lft_token=curr;
        NextnToken (&curr,2);//skip left and ","
        rgt_token=curr;
        NextnToken (&curr,2);//skip right and ")"

    }
    else{
        LogError (logger,curr,"unexpected token");
        return ParseError (curr);
    }

    lft_var=GetVarByToken (lft_token,vars,logger);
    rgt_var=GetVarByToken (rgt_token,vars,logger);

    g_operator (gen,var,lft_var,rgt_var,opr,hash);

    NextToken (&curr);// skip ";"

    cParseStatus s;
    s.NextTokenToParse=curr;
    return s;

}

cParseStatus ParseIF (cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    char *hash=curr->Hash;

    NextToken (&curr);

    curr=ParseContidion(curr,logger,vars,gen).NextTokenToParse;

    g_if(gen,hash,g_if_start);

    curr=ParseToken(curr,logger,vars,gen).NextTokenToParse;

    g_if(gen,hash,g_if_else);

    if(CheckTokenSeq (curr,NULL,"k",ckeyword_else)){
        NextToken (&curr);
        curr=ParseToken(curr,logger,vars,gen).NextTokenToParse;
    }

    g_if(gen,hash,g_if_end);

    cParseStatus s;
    s.NextTokenToParse=curr;
    return s;
}

cParseStatus ParseWhile(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    char* hash=curr->Hash;

    NextToken (&curr);

    g_while (gen,hash,g_while_start);

    curr=ParseContidion(curr,logger,vars,gen).NextTokenToParse;

    g_while (gen,hash,g_while_condition);

    curr=ParseToken(curr,logger,vars,gen).NextTokenToParse;

    g_while (gen,hash,g_while_end);

    cParseStatus s;
    s.NextTokenToParse=curr;
    return s;
}

cParseStatus ParseContidion(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    cToken *lft_token=NULL,*rgt_token=NULL;
    cvar   *lft_var,*rgt_var;
    cOperator op;
    cToken* end_of_cond;

    char* hash=curr->Hash;
    if(!CheckTokenSeq (curr,logger,"("))
        return ParseError(curr);

    end_of_cond=FindTokenPair (curr,'(',')');

    if (  cEot(end_of_cond) || end_of_cond->Position.row!=curr->Position.row) {
        LogError (logger,curr,"invalid condition");
        return ParseError(curr);
    }

    NextToken (&curr);//skip "("

    if(!CheckTokenSeq (curr,logger,"i"))
        return ParseError(curr);

    lft_token=curr;
    NextToken (&curr);//skip left id

    if(CheckTokenSeq (curr,NULL,"o",coperator_all)) {

    op=(cOperator)curr->data.i;
    NextToken (&curr);//skip operator

    if(!CheckTokenSeq (curr,logger,"i)"))
        return ParseError(curr);

    rgt_token=curr;
    NextToken (&curr);//skip right id

    } else if(!CheckTokenSeq (curr,logger,")"))
        return ParseError(curr);
    else op=coperator_add;

    lft_var=GetVarByToken (lft_token,vars,logger);
    rgt_var=GetVarByToken (rgt_token,vars,logger);

    g_operator (gen,NULL,lft_var,rgt_var,op,hash);

    NextToken (&curr);//skip ")"

    cParseStatus s;
    s.NextTokenToParse=curr;
    return s;
}

cParseStatus ParseIO(cToken* curr,clog* logger,cvar_list* vars,cgenerator* gen)
{
    if(!CheckTokenSeq (curr,logger,"k(i);",ckeyword_io))
        return ParseError(curr);

    cToken* id=curr;
    NextnToken (&id,2);

    cvar* var;

    if(!(var=cvarGetItem (vars,id->data.s))){
        LogError (logger,id,"ID is not defined");
        return ParseError(curr);
    }

    g_io (gen,var,(cKeyword)curr->data.i);

    NextnToken (&curr,5);
    cParseStatus s;
    s.NextTokenToParse=curr;
    return s;
}

cParseStatus ParseError(cToken* curr)
{
    cParseStatus s;
    cToken *a , *b;

    //Chance 1 : first ";" symbol occurance
    a=FindNextToken (curr,';',';');
    if(a!=NULL)
        NextToken (&a);

    //Chance 2 : end of line
    b=curr;
    while(b->NextToken->Position.row==b->Position.row)
        NextToken (&b);
    NextToken (&b);


    if( b->Position.offset < a->Position.offset && !cEot(b) )
        s.NextTokenToParse=b;
    else if(!cEot(a))
        s.NextTokenToParse=a;
    else s.NextTokenToParse=b;


    return s;
}

