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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cgenerator.h"
#include "cglobal.h"
#include "cvar.h"

void g_init(cgenerator* g)
{
    g->count=0;
}

void g_append(cgenerator* g, char* data)
{
    g->Items[g->count++]=_(data);
}

void g_comment(cgenerator* g, char* msg)
{
    g_append (g,__("\n## %",msg));
}

void g_main(cgenerator* g,g_program_status status)
{
    switch(status){
    case g_main_start:
        g_comment (g,"application main entry point");
        g_append (g,".text");
        g_append(g,"main:");
        break;
    case g_main_end:
        g_comment(g,"tell system to terminate application");
        g_append (g,"exit:");
        g_append (g,"li $v0,10");
        g_append(g,"syscall");
        g_comment (g,"end of application main entry point");
        break;
    default:
        g_comment (g,"error! unknown main stat!");
        break;
    }
}

void g_operator(cgenerator* g, cvar*dst,cvar* left,cvar* right,cOperator op,char* hash)
{
    static int2string_map_item cGenOperators[]={

        {"add",coperator_add},
        {"sub",coperator_sub},
        {"mul",coperator_multiply},
        {"div",coperator_divide},
        {"rem",coperator_modolus},

        {"seq",coperator_equals},
        {"sne",coperator_notequal},
        {"sge",coperator_greaterqual},
        {"sgt",coperator_greater},
        {"sle",coperator_lessequal},
        {"slt",coperator_less},

        {"or",coperator_or},
        {"or",coperator_logical_or},
        {"and",coperator_and},
        {"and",coperator_logical_and},

        {"",-1}
    };

    //Load vars to registers
    //Currently we assume every variable is a word
    g_comment (g,"Load variables");

    if(left!=NULL)
        g_append (g,__("lw $t0,var_%",left->Name));
    else
        g_append(g,__("move $t0,$0"));

    if(right!=NULL)
        g_append (g,__("lw $t1,var_%",right->Name));
    else
        g_append(g,__("move $t1,$0"));


    char*  command;
    if(command=GetMapKey (cGenOperators,op)){

        g_comment (g,command);

        g_append(g,__("% $t2,$t0,$t1",command));
    }
    else if ( op & (coperator_min|coperator_max) ) {

        g_comment(g,"conditional assignment");

        if(op == coperator_max)
            g_append(g,"sub $t2,$t0,$t1") ;
        else
            g_append(g,"sub $t2,$t1,$t0") ;

        g_append(g,__("bltz $t2,CondA_%",hash));
        g_append(g,"move $t2,$t0");
        g_append(g,__("j CondB_%",hash));
        g_append(g,__("CondA_%:",hash));
        g_append(g,"move $t2,$t1");
        g_append(g,__("CondB_%:",hash));

    } else
        g_comment (g,"unknown operator!");

    if(dst!=NULL)
        g_append (g,__("sw $t2,var_%",dst->Name));
}

void g_if(cgenerator* g,char* hash,g_if_status status)
{
    switch(status)
    {
    case g_if_start:
        g_comment (g,"start of if");
        g_append(g,__("beqz $t2,IFElse_%",hash));
        break;
    case g_if_else:
        g_append(g,__("j IFEnd_%",hash));
        g_comment (g,"IF Else part");
        g_append (g,__("IFElse_%:",hash));
        break;
    case g_if_end:
        g_comment (g,"End of if");
        g_append (g,__("IFEnd_%:",hash));
        break;
    }
}

void g_while(cgenerator* g,char* hash,g_if_status status)
{
    switch(status)
    {
    case g_while_start:
        g_comment (g,"While Start");
        g_append (g,__("WhileStart_%:",hash));
        break;
    case g_while_condition:
        g_comment (g,"Check while condition");
        g_append (g,__("beqz $t2,WhileEnd_%",hash));
        break;

    case g_while_end:
        g_append(g,__("j WhileStart_%",hash));
        g_comment(g,"End of while");
        g_append (g,__("WhileEnd_%:",hash));
        break;
    }
}

void g_io(cgenerator* g,cvar* var,cKeyword type)
{
    switch(type)
    {

    case ckeyword_printi:
        g_comment (g,"print integer");
        g_append (g,"li $v0,1");
        g_append(g,__("lw $a0,var_%",var->Name));
        g_append(g,__("syscall"));
        break;
    case ckeyword_prints:
        g_comment (g,"print string");
        g_append (g,"li $v0,4");
        g_append(g,__("la $a0,var_%",var->Name));
        g_append(g,__("syscall"));
        break;
    case ckeyword_scani:
        g_comment (g,"read integer");
        g_append (g,"li $v0,5");
        g_append(g,__("syscall"));
        g_append (g,__("sw $v0,var_%",var->Name));
        break;
    default:
        g_comment (g,"invalid IO type");
    }
}

void g_vars(cgenerator*g , cvar_list* vars)
{
    cvar* v;
    g_comment (g,"Allocate and label memory for variables") ;
    g_append (g,".data");
    for( v=vars->first_item;v!=NULL;v=v->next){
        switch(v->Type){
        case cvar_int:
            g_append (g,__("var_%:\t.word %",v->Name,itoa(v->Value.i)));
            break;
        case cvar_string:
            g_append(g,__("var_%:\t.asciiz \"%\"",v->Name,v->Value.s));
            break;
        default:
            g_comment (g,__("error :unknown data type for %",v->Name));
        }

    }
}

void g_dump(cgenerator *g,FILE* stream)
{
    int i;
    for(i=0;i<g->count;i++)
        fprintf(stream,"%s\n",g->Items[i]);
}
