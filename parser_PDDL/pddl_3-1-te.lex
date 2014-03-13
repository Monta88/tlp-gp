/* Parser PDDL */
/* pddl_3-1.lex */
/* Authors : BENIER Alan, PRAT Nathan, LAGLEIZE Martin, JOMAA Montassar */

%{
	#include "heading.h"
	#include "tok.h"
	int yyerror(char * err);
%}

%%

"("								{return L_P;}
")"								{return R_P;}
"["								{return L_B;}
"?"								{return Q_M;}
"#t"							{return NS_T;}
"="								{return EQ;}
"-"								{return MINUS;}
"/"								{return SLASH;}
"*"								{return STAR;}
"+"								{return PLUS;}
">"								{return GT;}
"<"								{return LT;}
">="							{return GOE;}
"<="							{return LOE;}
"_"								{return UNSC;}
"."								{return DOT;}
[0-9]+							{return NUM;}
"define"						{return DEFINE;}
"domain"						{return DOMAIN;}
":requirements"					{return REQUIREMENTS;}
":strips"						{return STRIPS;}
":typing"						{return TYPING;}
":negative-preconditions"		{return NEGATIVE_PRECONDITIONS;}
":disjunctive-preconditions"	{return DISJUNCTIVE_PRECONDITIONS;}
":equality"						{return EQUALITY;}
":existential-preconditions"	{return EXISTENTIAL_PRECONDITIONS;}
":universal-preconditions"		{return UNIVERSAL_PRECONDITIONS;}
":quantified-preconditions"		{return QUANTIFIED_PRECONDITIONS;}
":conditional-effects"			{return CONDITIONAL_EFFECTS;}
":fluents"						{return FLUENTS;}
":numeric-fluents"				{return NUMERIC_FLUENTS;}
":adl"							{return ADL;}
":durative-actions"				{return DURACTIVE_ACTIONS;}
":duration-inequalities"		{return DURATION_INEQUALITIES;}
":continuous-effects"			{return CONTINUOUS_EFFECTS;}
":derived-predicates"			{return DERIVED_PREDICATES;}
":timed-initial-literals"		{return TIMED_INITIAL_LITERALS;}
":preferences"					{return PREFERENCES;}
":constraints"					{return CONSTRAINTS;}
":action-costs"					{return ACTION_COSTS;}
":temporally-extended"			{return TEMPORALLY_EXTENDED;}
":types"						{return TYPES;}
":constants"					{return CONSTANTS;}
":predicates"					{return PREDICATES;}
":functions"					{return FUNCTIONS;}
"number"						{return NUMBER;}
"object"						{return OBJECT;}
"either"						{return EITHER;}
":action"						{return ACTION;}
":parameters"					{return PARAMETERS;}
":precondition"					{return PRECONDITION;}
":effect"						{return EFFECT;}
"and"							{return AND;}
"forall"						{return FORALL;}
"preference"					{return PREFERENCE;}
"or"							{return OR;}
"not"							{return NOT;}
"imply"							{return IMPLY;}
"exists"						{return EXISTS;}
"when"							{return WHEN;}
"assign"						{return ASSIGN;}
"undefined"						{return UNDEFINED;}
"scale-up"						{return SCALE_UP;}
"scale-down"					{return SCALE_DOWN;}
"increase"						{return INCREASE;}
"decrease"						{return DECREASE;}
":durative-action"				{return DURATIVE_ACTION;}
":duration"						{return DURATION;}
":condition"					{return CONDITION;}
"at"							{return AT;} // must be under NAME if "at" can be a predicate for example
"over"							{return OVER;}
"start"							{return START;}
"end"							{return END;}
"all"							{return ALL;}
"?duration"						{return Q_M_DURATION;}
":derived"						{return DERIVED;}
"problem"						{return PROBLEM;}
":domain"						{return DD_DOMAIN;}
":objects"						{return OBJECTS;}
":init"							{return INIT;}
":goal"							{return GOAL;}
"always"						{return ALWAYS;}
"sometime"						{return SOMETIME;}
"within"						{return WITHIN;}
"at-most-once"					{return AT_MOST_ONCE;}
"sometime-after"				{return SOMETIME_AFTER;}
"sometime-before"				{return SOMETIME_BEFORE;}
"always-within"					{return ALWAYS_WITHIN;}
"hold-during"					{return HOLD_DURING;}
"hold-after"					{return HOLD_AFTER;}
":metric"						{return METRIC;}
"minimize"						{return MINIMIZE;}
"maximize"						{return MAXIMIZE;}
"total-time"					{return TOTAL_TIME;}
"is-violated"					{return IS_VIOLATED;}
":length"						{return LENGTH;}
":serial"						{return SERIAL;}
"parallel"						{return PARALLEL;}
":timepoints"					{return TIMEPOINTS;}
":timealiases"					{return TIMEALIASES;}
":timeconstraints"				{return TIMECONSTRAINTS;}
[a-zA-Z][a-zA-Z0-9_^-]*			{yylval.sval=strdup(yytext); return NAME;}
[;]+.*[\n]						{++yylineno;} /* ignore comments */
[\n]							{++yylineno;} /* ignore excessive new lines */
[\t]+							/* ignore excessive tabulations */
[ ]+							/* ignore excessive spaces */
.								{std::cerr << "SCANNER "; yyerror(""); exit(1);}

