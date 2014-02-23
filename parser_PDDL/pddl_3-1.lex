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
"?"					{return Q_M;}
"="					{return EQ;}
"\n"				{return N_L;}
"\t"				{return TAB;}
"-"					{return MINUS;}
"/"					{return SLASH;}
"*"					{return STAR;}
"+"					{return PLUS;}
">"					{return GT;}
"<"					{return LT;}
">="				{return GOE;}
"<="				{return LOE;}
"_"					{return UNSC;}
"."					{return DOT;}
[a-zA-Z]			{return LETTER;}
[0-9]				{return DIGIT;}
"define"			{return DEFINE;}
"domain"			{return DOMAIN;}
":requirements"		{return REQUIREMENTS;}
":types"			{return TYPES;}
":constants"		{return CONSTANTS;}
":predicates"		{return PREDICATES;}
":functions"		{return FUNCTIONS;}
"number"			{return NUMBER;}
":constraints"		{return CONSTRAINTS;}
"object"			{return OBJECT;}
"either"			{return EITHER;}
":action"			{return ACTION;}
":parameters"		{return PARAMETERS;}
":precondition"		{return PRECONDITION;}
":effect"			{return EFFECT;}
"and"				{return AND;}
"forall"			{return FORALL;}
"preference"		{return PREFERENCE;}
"or"				{return OR;}
"not"				{return NOT;}
"imply"				{return IMPLY;}
"exists"			{return EXISTS;}
"when"				{return WHEN;}
"assign"			{return ASSIGN;}
"undefined"			{return UNDEFINED;}
"scale-up"			{return SCALE_UP;}
"scale-down"		{return SCALE_DOWN;}
"increase"			{return INCREASE;}
"decrease"			{return DECREASE;}
":durative-action"	{return DURATIVE_ACTION;}
":duration"			{return DURATION;}
":condition"		{return CONDITION;}
"at"				{return AT;}
"over"				{return OVER;}
"start"				{return START;}
"end"				{return END;}
"all"				{return ALL;}
"?duration"			{return Q_M_DURATION;}
":derived"			{return DERIVED;}
"problem"			{return PROBLEM;}
":domain"			{return DD_DOMAIN;}
":objects"			{return OBJECTS;}
":init"				{return INIT;}
":goal"				{return GOAL;}
"end"				{return END;}
"always"			{return ALWAYS;}
"sometime"			{return SOMETIME;}
"within"			{return WITHIN;}
"at-most-once"		{return AT_MOST_ONCE;}
"sometime-after"	{return SOMETIME_AFTER;}
"sometime-before"	{return SOMETIME_BEFORE;}
"always-within"		{return ALWAYS_WITHIN;}
"hold-during"		{return HOLD_DURING;}
"hold-after"		{return HOLD_AFTER;}
":metric"			{return METRIC;}
"minimize"			{return MINIMIZE;}
"maximize"			{return MAXIMIZE;}
"total-time"		{return TOTAL_TIME;}
"is-violated"		{return IS_VIOLATED;}
":length"			{return LENGTH;}
":serial"			{return SERIAL;}
"parallel"			{return PARALLEL;}

[ \t]*		{}
[\n]		{ yylineno++;	}

.		{ std::cerr << "SCANNER "; yyerror(""); exit(1);	}

