%{
#include "my.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
#include "my.tab.h" 
%}
%%
[ \t] ;
[(]		{ return LB; }
[)]		{ return RB; }
define	{ return DEFINE; }
domain { return DOMAINE; }
problem { return PROBLEM; }
:requirements	{ return REQUIREMENTS; }
:types			{ return TYPES; }
:constants		{ return CONSTANT; }
:predicates		{ return PREDICATES; }
:functions		{ return FUNCTIONS; }
:action			{ return ACTION; }
:durative-action { return DURATIVEACTION; }
[a-zA-Z0-9]+ { yylval.sval=yytext; return NAME; }
.		;
