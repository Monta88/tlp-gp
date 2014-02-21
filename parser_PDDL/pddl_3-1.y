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
input:
		/* empty */
		| exp	{ cout << "Result: " << $1 << endl; }
		;

exp:
		INTEGER_LITERAL	{ $$ = $1; }
		| exp PLUS exp	{ $$ = $1 + $3; }
		| exp MULT exp	{ $$ = $1 * $3; }
		;
		
/***************************************************/
/************** Domain Description *****************/
/***************************************************/		

domain:
		/* empty */
		|
		;
/* réfléchir aux options */

require-def:
		L_P DD REQUIREMENTS nvList_require-key R_P {}
		;
		
nvList_require-key:
		require-key list_require-key {}
		;
		
list_require-key:
		/* empty */
		| require-key list_require_key {}
		;
		
require-key: /* plutard */
		;
		
/* définition comme une fonction est bizarre et peut être faux implémenté comme ça */
types-def:
		L_P DD TYPES typed-list L_P x R_P R_P {$4 /* corespond à x */}
		;
		
constantes-def:
		L_P DD CONSTANTS typed-list L_P name R_P R_P {$4 /* corespond à name */}
		;

predicates-def:
		L_P DD PREDICATES nvList_atomic-formula-skeleton R_P {}
		;

nvList_atomic-formula-skeleton:
		atomic-formula-skeleton list_atomic-formula-skeleton {}
		;

list_atomic-formula-skeleton:
		/* empty */		
		| atomic-formula-skeleton list_atomic-formula-skeleton {}
		;
		
atomic-formula-skeleton:
		L_P predicate typed-list L_P variable R_P R_P {$5 /* corespond à name */}
		;
		
predicate:
		name {}
		;
		
variable:
		Q_M name {}
		;

atomic-function-skeleton:
		L_P function-symbol typed-list L_P variable R_P R_P {}
		;
		
function-symbol:
		name {}
		;
		
function-def:
		L_P DD FUNCTIONS function-typed-list L_P atomic-function_skeleton R_P R_P {}
		;
		
function-typed-list:
		/* empty */
		| /* x+ */
		| /* x+ */ MINUS type typed-list L_P /* x */ R_P
		;

function-type:
		/* number */
		| type
		;

constraints:
		L_P DD CONSTRAINTS con-GD R_P
		;
		
		
structure-def:
		action-def
		| durative-action-def
		| derived-def
		;
		
typed-list:
		/* x* */
		| /* x* */ MINUS type typed-list L_P /* x */ R_P
		;
		
primitive-type:
		name
		| /* object */
		;
		
type:
		primitive-type
		| L_P EITHER nvList_primitive-type R_P
		;
		
nvList_primitive-type:
		primitive-type list_primitive-type
		;
		
list_primitive-type:
		/* empty */
		| primitive-type list_primitive-type
		;
		
emptyOr:
		L_P R_P
		| /* x */
		;
		
action-def:
		L_P DD ACTION action-symbol N_L TAB DD PARAMETERS L_P typed-list L_P /* variable */ R_P R_P N_L TAB action-def-body R_P
		;
		
action-symbol:
		name
		;
		
action-def-body
		/* empty */
		| /* choix */
		;
		
pre-GD:
		pref-GD
		| L_P AND list_pre-GD R_P
		| L_P FORALL L_P typed-list L_P /* variable */ R_P R_P pre-GD R_P
		;
		
list_pre-GD:
		pre-GD list_pre-GD
		;
	
pref-GD:
		GD
		| L_P PREFERENCE /* choix */ GD R_P
		;
	
pref-name:
		name
		;
	
		
/***************************************************/
/************** Problem Description ****************/
/***************************************************/


/***************************************************/
/*********************** End ***********************/
/***************************************************/

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


