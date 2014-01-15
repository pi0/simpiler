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

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "ctoken.h"
#include "cparser.h"


int main(int argc,char* args[])
{
    char* inpname=NULL;
    char* outname=NULL;
    FILE* outstream;

    char* code;

    int DumpTokens=0;
    int DumpVars=0;


    //Parse arguments

    int i;
    for(i=0;i<argc;i++)
    {
        if(strlen(args[i])!=2)
            continue;

        switch(args[i][1])
        {
        case 'i' : inpname=args[++i]; break;
        case 'o' : outname=args[++i]; break;
        case 't' : DumpTokens=1;break;
        case 'v' : DumpVars=1;break;
        case 'h': ShowHelp ();return 0;
        }

    }

   //Read input file

   if(!inpname) {
       printf("Error: no input file specified , use -i filename\n");
       ShowHelp ();
       return 1;
   }


    code=ReadAllFile (inpname);

    if(!code){
        printf("Error : can't read %s\n",inpname);
        return 1;
    }

    //Open output stream

    if(!outname)
        outstream=stdout;
    else if (!(outstream=fopen(outname,"w"))) {
        printf ("Error : can't write to %s",outname);
        return 1;
    }


    cTokenList tokens=GetToken (code);
    if(DumpTokens)
        DumpTokenList (tokens,stdout);

    cgenerator gen;
    g_init(&gen);

    clog logger;
    LoggerInit (&logger);
    logger.out_stream=stderr;

    cvar_list vars;

    ParseTokens(&tokens,&logger,&vars,&gen);

    if(DumpVars)
        DumpVarList(&vars,stdout);

    g_dump(&gen,outstream);
    fclose (outstream);

    printf("Successful run ! \n");

    return 0;
}

void ShowHelp()
{
    printf  ("Usage : simpiler [OPTION] .... \n\n"
             "A simple c compiler for SPIM\n\n"
             "Arguments:\n"
             "-i file \t\t Input file\n"
             "-o file \t\t Output file\n"
             "-t \t\t\t Dumps token list to stdout\n"
             "-s\t\t\t Dumps symbols list to stdout\n"
             "-h \t\t\t Show this help\n\n"
             "argument -i is mondatory\n\n"
             "Report bugs to pyapar@gmail.com\n\n"
             );
}

char* ReadAllFile(const char* path)
{
    FILE* fh=fopen(path,"r+");

    if(!fh)return NULL;

    fseek(fh,0,SEEK_END);
    int size=ftell(fh);
    char* buff=(char*)malloc(size);
    fseek(fh,0,SEEK_SET);
    fread(buff,1,size,fh);
    return buff;
}
