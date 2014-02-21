/* Mini Calculator */
/* pddl_3-1.lex */

%{
#include "heading.h"
#include "tok.h"
int yyerror(char *s);
/*int yylineno = 1;*/ /* the flex generator duplicate this line, which generate an error when compiling with make */
%}

digit		[0-9]
int_const	{digit}+

%%

{int_const}	{ yylval.int_val = atoi(yytext); return INTEGER_LITERAL; }
"+"		{ yylval.op_val = new std::string(yytext); return PLUS; }
"*"		{ yylval.op_val = new std::string(yytext); return MULT; }

/* terminaux ici */
"("					{return L_P;}
")"					{return R_P;}
":"					{return DD;}
"?"					{return Q_M;}
"requirements"		{return REQUIREMENTS;}
"types"				{return TYPES;}
"constants"			{return CONSTANTS;}
"predicates"		{return PREDICATES;}
"functions"			{return FUNCTIONS;}
"-"					{return MINUS;}
"constraints"		{return CONSTRAINTS;}
"either"			{return EITHER;}
"action"			{return ACTION;}
"\n"				{return N_L;}
"\t"				{return TAB;}
"parameters"		{return PARAMETERS;}
"and"				{return AND;}
"forall"			{return FORALL;}
"preference"		{return PREFERENCE;}


[ \t]*		{}
[\n]		{ yylineno++;	}

.		{ std::cerr << "SCANNER "; yyerror(""); exit(1);	}

