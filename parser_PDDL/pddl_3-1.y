/* Mini Calculator */
/* pddl_3-1.y */

%{
#include "heading.h"
int yyerror(char *s);
int yylex(void);
%}

%union{
  int		int_val;
  string*	op_val;
}

%start	input 

%token	<int_val>	INTEGER_LITERAL
%type	<int_val>	exp
%left	PLUS
%left	MULT

%left L_P
%left R_P
%left DD
%left REQ
%left TYP


%%

/* completer ici */
input:		/* empty */
		| exp	{ cout << "Result: " << $1 << endl; }
		;

exp:		INTEGER_LITERAL	{ $$ = $1; }
		| exp PLUS exp	{ $$ = $1 + $3; }
		| exp MULT exp	{ $$ = $1 * $3; }
		;
domain:		/* empty */
		|
		;
/* réfléchir aux options */

require-def:	L_P DD REQ nvList_require-key R_P {}
		;
		
nvList_require-key:		require-key list_require-key {}
		;
		
list_require-key:		/* empty */
		| require-key list_require_key {}
		;
		
require-key: /* plutard */
		;
		
/* définition comme une fonction est bizarre et peut être faux implémenté comme ça */
types-def:		L_P DD TYP typed-list L_P x R_P R_P {$4 /* corespond à x */}
		;
	
/* continuer */	
:		
		|
		;

:		
		|
		;

:		
		|
		;

:		
		|
		;
		
%%

int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c
  
  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
}

int yyerror(char *s)
{
  return yyerror(string(s));
}


