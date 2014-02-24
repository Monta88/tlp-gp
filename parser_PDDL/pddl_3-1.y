/* Parser PDDL */
/* pddl_3-1.y */

%{
	#include "heading.h"
	#define YYSTYPE std::string
	int yyerror(char *s);
	int yylex(void);
%}

%union {
	char *sval;
}

%start begin

%token L_P
%token R_P
%token Q_M
%token EQ
%token MINUS
%token SLASH
%token STAR
%token PLUS
%token GT
%token LT
%token GOE
%token LOE
%token UNSC
%token DOT
%token LETTER
%token DIGIT
%token NUMBER
%token DEFINE
%token DOMAIN
%token REQUIREMENTS
%token STRIPS
%token TYPING
%token NEGATIVE_PRECONDITIONS
%token DISJUNCTIVE_PRECONDITIONS
%token EQUALITY
%token EXISTENTIAL_PRECONDITIONS
%token UNIVERSAL_PRECONDITIONS
%token QUANTIFIED_PRECONDITIONS
%token CONDITIONAL_EFFECTS
%token FLUENTS
%token NUMERIC_FLUENTS
%token ADL
%token DURACTIVE_ACTIONS
%token DURATION_INEQUALITIES
%token CONTINUOUS_EFFECTS
%token DERIVED_PREDICATES
%token TIMED_INITIAL_LITERALS
%token PREFERENCES
%token CONSTRAINTS
%token ACTION_COSTS
%token TYPES
%token CONSTANTS
%token PREDICATES
%token FUNCTIONS
%token NUM
%token OBJECT
%token EITHER
%token ACTION
%token PARAMETERS
%token PRECONDITION
%token EFFECT
%token AND
%token FORALL
%token PREFERENCE
%token OR
%token NOT
%token IMPLY
%token EXISTS
%token WHEN
%token ASSIGN
%token UNDEFINED
%token SCALE_UP
%token SCALE_DOWN
%token INCREASE
%token DECREASE
%token DURATIVE_ACTION
%token DURATION
%token CONDITION
%token AT
%token OVER
%token START
%token END
%token ALL
%token Q_M_DURATION
%token DERIVED
%token PROBLEM
%token DD_DOMAIN
%token OBJECTS
%token INIT
%token GOAL
%token ALWAYS
%token SOMETIME
%token WITHIN
%token AT_MOST_ONCE
%token SOMETIME_AFTER
%token SOMETIME_BEFORE
%token ALWAYS_WITHIN
%token HOLD_DURING
%token HOLD_AFTER
%token METRIC
%token MINIMIZE
%token MAXIMIZE
%token TOTAL_TIME
%token IS_VIOLATED
%token LENGTH
%token SERIAL
%token PARALLEL
%token <sval> NAME

%%

begin:
		domain {YYACCEPT;}
		| problem {YYACCEPT;}
		;


/***************************************************/
/************** Domain Description *****************/
/***************************************************/		

/* <domain> */

domain:
		L_P DEFINE L_P DOMAIN name R_P domain_option1
		;

domain_option1:
		domain_option2
		| require-def
		;

domain_option2:
		domain_option3
		| types-def
		;

domain_option3:
		domain_option4
		| constants-def
		;

domain_option4:
		domain_option5
		| predicates-def
		;

domain_option5:
		domain_option6
		| functions-def
		;

domain_option6:
		domain_option7
		| constraints
		;

domain_option7:
		list_structure-def R_P
		;

list_structure-def:
		/* empty */
		| structure-def list_structure-def
		;

name:
		NAME
		;

/* <require-def> */

require-def:
		L_P REQUIREMENTS nvList_require-key R_P
		;
		
nvList_require-key:
		require-key list_require-key
		;
		
list_require-key:
		/* empty */
		| require-key list_require-key
		;
		
/* <require-key> */

require-key:
		STRIPS
		| TYPING
		| NEGATIVE_PRECONDITIONS
		| DISJUNCTIVE_PRECONDITIONS
		| EQUALITY
		| EXISTENTIAL_PRECONDITIONS
		| UNIVERSAL_PRECONDITIONS
		| QUANTIFIED_PRECONDITIONS
		| CONDITIONAL_EFFECTS
		| FLUENTS
		| NUMERIC_FLUENTS
		| ADL
		| DURACTIVE_ACTIONS
		| DURATION_INEQUALITIES
		| CONTINUOUS_EFFECTS
		| DERIVED_PREDICATES
		| TIMED_INITIAL_LITERALS
		| PREFERENCES
		| CONSTRAINTS
		| ACTION_COSTS
		;
		
/* <types-def> */
types-def:
		L_P TYPES typed-list R_P //fn
		;
		
/* <contants-def> */

constants-def:
		L_P CONSTANTS typed-list R_P //fn
		;

/* <predicates-def> */

predicates-def:
		L_P PREDICATES nvList_atomic-formula-skeleton R_P
		;

nvList_atomic-formula-skeleton:
		atomic-formula-skeleton list_atomic-formula-skeleton
		;

list_atomic-formula-skeleton:
		/* empty */		
		| atomic-formula-skeleton list_atomic-formula-skeleton
		;
		
/* <atomic formula skeleton> */
		
atomic-formula-skeleton:
		L_P predicate typed-list R_P //fn
		;
		
/* <predicate> */

predicate:
		name
		;
		
/* <variable> */

variable:
		Q_M name
		;

/* <atomic function skeleton> */

atomic-function-skeleton:
		L_P function-symbol typed-list R_P //fn
		;
		
/* <function-symbol> */

function-symbol:
		name
		;
		
/* <functions-def> */

functions-def:
		L_P FUNCTIONS function-typed-list L_P atomic-function-skeleton R_P R_P
		;
		
/* <function typed list (x)> */

function-typed-list:
		/* empty */
		| nvList_name
		| nvList_name MINUS type typed-list L_P name R_P //fn
		;

nvList_name:
		name list_name
		;

/* <function type> */

function-type:
		NUMBER
		| type
		;

/* <constraints> */

constraints:
		L_P CONSTRAINTS con-GD R_P
		;
		
/* <structure-def> */

structure-def:
		action-def
		| durative-action-def
		| derived-def
		;
		
/* <typed list (x)> */

typed-list: //fn
		list_name
		| list_name MINUS type typed-list L_P name R_P
		;
		
/* <primitive-type> */

primitive-type:
		name
		| OBJECT
		;
		
/* <type> */

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
		
/* <emptyOr (x)> */

emptyOr: //fn
		L_P R_P
		| name
		;
		
/* <action-def> */

action-def:
		L_P ACTION action-symbol PARAMETERS L_P typed-list R_P action-def-body R_P //fn
		;
		
/* <action-symbol> */

action-symbol:
		name
		;
		
/* <action-def body> */

action-def-body:
		action-def-body_option1
		;
		
action-def-body_option1:
		action-def-body_option2
		| PRECONDITION emptyOr action-def-body_option2 //fn
		;

action-def-body_option2:
		/* empty */
		| EFFECT emptyOr //fn
		;
		
/* <pre-GD> */

pre-GD:
		pref-GD
		| L_P AND list_pre-GD R_P
		| L_P FORALL L_P typed-list R_P pre-GD R_P //fn
		;
		
list_pre-GD:
		pre-GD list_pre-GD
		;

/* <pref-GD> */
	
pref-GD:
		GD
		| L_P PREFERENCE pref-GD_option1
		;

pref-GD_option1:
		 GD R_P
		| pref-name GD R_P
		;

/* <pref-name> */

pref-name:
		name
		;
		
/* <GD> */

GD:
		atomic-formula //fn
		| literal //fn
		| L_P AND list_GD R_P
		| L_P OR list_GD R_P
		| L_P NOT GD R_P
		| L_P IMPLY GD GD R_P
		| L_P EXISTS L_P L_P typed-list R_P R_P GD R_P //fn
		| L_P FORALL L_P L_P typed-list R_P R_P GD R_P //fn
		| f-comp
		;
		
list_GD:
		/* empty */
		| GD list_GD
		;

/* <f-comp> */

f-comp:
		L_P binary-comp f-exp f-exp R_P
		;

/* <litteral (t)> */

literal: //fn
		atomic-formula //fn
		| L_P NOT atomic-formula //fn
		;

/* <atomic formula (t)> */

atomic-formula: //fn
		L_P predicate list_name R_P
		| L_P EQ name name R_P
		;

/* <term> */

term:
	name
	| variable
	| function-term
	;

/* <function-term> */

function-term:
		L_P function-symbol list_term R_P
		;

list_term:
		/* empty */
		| term list_term
		;

/* <f-exp> */

f-exp:
		number
		| L_P binary-op f-exp f-exp R_P
		| L_P multi-op f-exp nvList_f-exp R_P
		| L_P MINUS f-exp R_P
		| f-head
		;

number:
		NUM
		;

nvList_f-exp:
		f-exp list_f-exp
		;

list_f-exp:
		/* empty */
		| f-exp list_f-exp
		;

/* <f-head> */

f-head:
		L_P function-symbol list_term R_P
		| function-symbol
		;

/* <binary-op> */

binary-op:
		multi-op
		| MINUS
		| SLASH
		;

/* <multi-op> */

multi-op:
		STAR
		| PLUS
		;

/* <binary-comp> */

binary-comp:
		GT
		| LT
		| EQ
		| GOE
		| LOE
		;

/* <name> */

name:
	letter list_any-char
	;

/* <letter> */

letter:
		LETTER
		;

/* <any char> */

any-char:
		letter
		| digit
		| MINUS
		| UNSC
		;

list_any-char:
		/* empty */
		| any-char list_any-char
		;

/* <digit> */

digit:
		DIGIT
		;

/* <decimal> */

decimal:
		DOT nvList_digit
		;

nvList_digit:
		digit list_digit
		;

list_digit:
		/* empty */
		| digit list_digit
		;

/* <effect> */

effect:
		L_P AND list_c-effect R_P
		| c-effect
		;

list_c-effect:
		/* empty */
		| c-effect list_c-effect
		;

/* <c-effect> */

c-effect:
		L_P FORALL L_P typed-list R_P effect R_P //fn
		| L_P WHEN GD cond-effect R_P
		| p-effect
		;

/* <p-effect> */

p-effect:
		L_P NOT atomic-formula R_P //fn
		| atomic-formula //fn
		| L_P assign-op f-head f-exp
		| L_P ASSIGN function-term term R_P
		| L_P ASSIGN function-term UNDEFINED R_P
		;

/* <cond-effect> */

cond-effect:
		L_P AND list_p-effect R_P
		| p-effect
		;

list_p-effect:
		/* empty */
		| p-effect list_p-effect
		;

/* <assign-op> */

assign-op:
		ASSIGN
		| SCALE_UP
		| SCALE_DOWN
		| INCREASE
		| DECREASE
		;

/* <durative-action-def> */

durative-action-def:
		L_P DURATIVE_ACTION da-symbol PARAMETERS L_P typed-list R_P da-def-body R_P //fn
		;

/* <da-symbol> */

da-symbol:
		name
		;

/* <da-def body> */

da-def-body:
		DURATION duration-constraint CONDITION emptyOr EFFECT emptyOr //fn
		;

/* <da-GD> */

da-GD:
		pref-timed-GD
		| L_P AND list_da-GD R_P
		| L_P FORALL L_P typed-list R_P da-GD R_P //fn
		;

list_da-GD:
		/* empty */
		| da-GD list_da-GD
		;

/* <pref-timed-GD> */

pref-timed-GD:
		timed-GD
		| L_P PREFERENCE pref-name_option1
		;

pref-name_option1:
		timed-GD R_P
		| pref-name timed-GD R_P
		;

/* <timed-GD> */

timed-GD:
		L_P AT time-specifier GD R_P
		| L_P OVER interval GD R_P
		;

/* <time-specifier> */

time-specifier:
		START
		| END
		;

/* <interval> */

interval:
		ALL
		;

/* <duration-constraint> */

duration-constraint:
		L_P AND nvList_simple-duration-constraint R_P
		| L_P R_P
		| simple-duration-constraint
		;

nvList_simple-duration-constraint:
		simple-duration-constraint list_simple-duration-constraint
		;

list_simple-duration-constraint:
		/* empty */
		| simple-duration-constraint list_simple-duration-constraint
		;

/* <simple-duration-constraint> */

simple-duration-constraint:
		L_P d-op Q_M_DURATION d-value R_P
		| L_P AT time-specifier simple-duration-constraint R_P
		;

/* <d-op> */

d-op:
		LOE
		| GOE
		| EQ
		;

/* <d-value> */

d-value:
		NUMBER
		| f-exp
		;

/* <da-effect> */

da-effect:
		L_P AND list_da-effect R_P
		| timed-effect
		| L_P FORALL L_P typed-list R_P da-effect R_P //fn
		| L_P WHEN da-GD timed-effect R_P
		;

list_da-effect:
		/* empty */
		| da-effect list_da-effect
		;

/* <timed-effect> */

timed-effect:
		L_P AT time-specifier cond-effect R_P
		| L_P AT time-specifier f-assign-da R_P
		| L_P assign-op-t f-head f-exp-t R_P
		;

/* <f-assign-da> */

f-assign-da:
		L_P assign-op f-head f-exp-da R_P
		;

/* <f-exp-da> */

f-exp-da:
		L_P binary-op f-exp-da f-exp-da R_P
		| L_P multi-op f-exp-da nvList_f-exp-da R_P
		| L_P MINUS f-exp-da R_P
		| Q_M_DURATION
		| f-exp
		;

nvList_f-exp-da:
		f-exp-da list_f-exp-da
		;

list_f-exp-da:
		/* empty */
		| f-exp-da list_f-exp-da
		;

/* <assign-op-t> */

assign-op-t:
		INCREASE
		| DECREASE
		;

/* <f-exp-t> */

f-exp-t:
		L_P STAR f-exp //todo R_P
		| L_P STAR //todo f-exp R_P
		| //todo
		;

/* <derived-def> */

derived-def:
		L_P DERIVED atomic-formula-skeleton GD R_P
		;


/***************************************************/
/************** Problem Description ****************/
/***************************************************/

/* <problem> */

problem:
		L_P DEFINE L_P PROBLEM name R_P L_P DD_DOMAIN name R_P problem_option1
		;

problem_option1:
		problem_option2
		| require-def problem_option2
		;

problem_option2:
		problem_option3
		| object-declaration problem_option3
		;

problem_option3:
		init goal problem_option4
		| init goal constraints problem_option4
		;

problem_option4:
		problem_option5
		| metric-spec problem_option5
		;

problem_option5:
		R_P
		| length-spec R_P

/* <object declaration> */

object-declaration:
		L_P OBJECTS typed-list R_P //fn
		;

/* <init> */

init:
		L_P INIT list_init-el R_P
		;

list_init-el:
		/* empty */
		| init-el list_init-el
		;

/* <init-el> */

init-el:
		literal //fn
		| L_P AT number literal R_P //fn
		| L_P EQ basic-function-term number R_P
		| L_P EQ basic-function-term name R_P
		;

/* <basic-function-term> */

basic-function-term:
		function-symbol
		| L_P function-symbol list_name R_P
		;

list_name:
		/* empty */
		| name list_name
		;

/* <goal> */

goal:
		L_P GOAL pre-GD
		;

/* <contraints> */
/* is contraints different between domain definition and problem definition ? */

/* <pref-con-GD> */

pref-con-GD:
		L_P AND list_pref-con-GD R_P
		| L_P FORALL L_P typed-list L_P variable R_P R_P pref-con-GD R_P
		| L_P PREFERENCE pref-con-GD_option1
		| con-GD
		;

pref-con-GD_option1:
		con-GD R_P
		| pref-name con-GD R_P
		;

list_pref-con-GD:
		/* empty */
		| pref-con-GD list_pref-con-GD
		;

/* <con-GD> */

con-GD:
		L_P AND list_con-GD R_P
		| L_P FORALL L_P typed-list R_P con-GD R_P //fn
		| L_P AT END GD R_P
		| L_P ALWAYS con2-GD R_P
		| L_P SOMETIME con2-GD R_P
		| L_P WITHIN number con2-GD R_P
		| L_P AT_MOST_ONCE con2-GD con2-GD R_P
		| L_P SOMETIME_AFTER con2-GD con2-GD R_P
		| L_P SOMETIME_BEFORE con2-GD con2-GD R_P
		| L_P ALWAYS_WITHIN number con2-GD con2-GD R_P
		| L_P HOLD_DURING number number con2-GD R_P
		| L_P HOLD_AFTER number con2-GD R_P
		;

list_con-GD:
		/* empty */
		| con-GD list_con-GD
		;

/* <con2-GD> */

con2-GD:
		con-GD
		| GD
		;

/* <metric-spec> */

metric-spec:
		L_P METRIC optimization metric-f-exp R_P
		;

/* <optimization> */

optimization:
		MINIMIZE
		| MAXIMIZE
		;

/* <metric-f-exp> */

metric-f-exp:
		L_P binary-op metric-f-exp metric-f-exp R_P
		| L_P multi-op metric-f-exp nvList_metric-f-exp R_P
		| L_P MINUS metric-f-exp R_P
		| number
		| L_P function-symbol list_name R_P
		| function-symbol
		| TOTAL_TIME
		| L_P IS_VIOLATED pref-name R_P
		;

nvList_metric-f-exp:
		metric-f-exp list_metric-f-exp
		;

list_metric-f-exp:
		/* empty */
		| metric-f-exp list_metric-f-exp
		;

/* <length-spec> */

length-spec:
		L_P LENGTH length-spec_option1
		;

length-spec_option1:
		length-spec_option2
		| L_P SERIAL integer R_P length-spec_option2
		;

length-spec_option2:
		R_P
		| L_P PARALLEL integer R_P R_P
		;

integer:
		NUM
		;


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


