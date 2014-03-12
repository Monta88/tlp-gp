/* Parser PDDL */
/* pddl_3-1.y */
/* Authors : BENIER Alan, PRAT Nathan, LAGLEIZE Martin, JOMAA Montassar */

%{
	#include "heading.h"
	#define YYSTYPE std::string
	#define YYINITDEPTH 2000 // can be a source the error "memory exhausted", attempt to increase this to fix this
	int yyerror(char *s);
	int yylex(void);
%}

%union {
	char *sval;
}

%start begin

%token L_P
%token R_P
%token L_B
%token Q_M
%token NS_T
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
%token TEMPORALLY_EXTENDED
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
%token TIMEPOINTS
%token TIMEALIASES
%token TIMECONSTRAINTS
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
	| require-def domain_option2
	;

domain_option2:
	domain_option3
	| types-def domain_option3 // require :typing
	;

domain_option3:
	domain_option4
	| constants-def domain_option4
	;

domain_option4:
	domain_option5
	| predicates-def domain_option5
	;

domain_option5:
	domain_option6
	| functions-def domain_option6 // require :fluents
	;

domain_option6:
	domain_option7
	| constraints-domain domain_option7
	;

domain_option7:
	list_structure-def R_P
	;

/* <name> */

name:
	NAME
	;

/* <require-def> */

require-def:
	L_P REQUIREMENTS nvList_require-key R_P
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
	| TEMPORALLY_EXTENDED
	;

/* <types-def> */
types-def:
	L_P TYPES typed-list_name R_P
	;

/* <contants-def> */

constants-def:
	L_P CONSTANTS typed-list_name R_P
	;

/* <predicates-def> */

predicates-def:
	L_P PREDICATES nvList_atomic-formula-skeleton R_P
	;

/* <atomic formula skeleton> */

atomic-formula-skeleton:
	L_P predicate typed-list_variable R_P
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
	L_P function-symbol typed-list_variable R_P
	;

/* <function-symbol> */

function-symbol:
	name
	;

/* <functions-def> */

functions-def:
	L_P FUNCTIONS function-typed-list_atomic-function-skeleton R_P // require :fluents
	;

/* <function type> */

function-type:
	NUMBER // require :numeric-fluents
	| type // require :typing + :object-fluents
	;

/* <constraints> */

constraints-domain:
	L_P CONSTRAINTS con-GD R_P // require :constraints
	;

/* <structure-def> */

structure-def:
	action-def
	| durative-action-def // require :durative-actions
	| te-durative-action-def // require :temporally-extended
	| derived-def // require :derived-predicates
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

/* <action-def> */

action-def:
	L_P ACTION action-symbol PARAMETERS L_P typed-list_variable R_P action-def-body R_P
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
	| PRECONDITION emptyOr_pre-GD action-def-body_option2
	;

action-def-body_option2:
	/* empty */
	| EFFECT emptyOr_effect
	;

/* <pre-GD> */

pre-GD:
	pref-GD
	| L_P AND list_pre-GD R_P
	| L_P FORALL L_P typed-list_variable R_P pre-GD R_P // require :universal-preconditions
	;

/* <pref-GD> */

pref-GD:
	GD
	| L_P PREFERENCE pref-GD_option1 // require :preferences
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
	atomic-formula_term
	| literal_term // require :negative-preconditions
	| L_P AND list_GD R_P
	| L_P OR list_GD R_P // require :disjunctive-preconditions
	| L_P NOT GD R_P // require :disjunctive-preconditions
	| L_P IMPLY GD GD R_P // require :disjunctive-preconditions
	| L_P EXISTS L_P L_P typed-list_variable R_P R_P GD R_P // require :existential-preconditions
	| L_P FORALL L_P L_P typed-list_variable R_P R_P GD R_P // require :universal-preconditions
	| f-comp // require :numeric-fluents
	;

/* <f-comp> */

f-comp:
	L_P binary-comp f-exp f-exp R_P
	;

/* <term> */

term:
	name
	| variable
	| function-term // require :object-fluents
	;

/* <function-term> */

function-term:
	L_P function-symbol list_term R_P // require :object-fluents
	;

/* <f-exp> */

f-exp:
	number // require :numeric-fluents
	| L_P binary-op f-exp f-exp R_P // require :numeric-fluents
	| L_P multi-op f-exp nvList_f-exp R_P // require :numeric-fluents
	| L_P MINUS f-exp R_P // require :numeric-fluents
	| f-head // require :numeric-fluents
	;

/* <number> */

number:
	NUM
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

/* <digit> */

digit:
	DIGIT
	;

/* <decimal> */

decimal:
	DOT nvList_digit
	;

/* <effect> */

effect:
	L_P AND list_c-effect R_P
	| c-effect
	;

/* <c-effect> */

c-effect:
	L_P FORALL L_P typed-list_variable R_P effect R_P// require :conditional-effects
	| L_P WHEN GD cond-effect R_P // require :conditional-effects
	| p-effect
	;

/* <p-effect> */

p-effect:
	L_P NOT atomic-formula_term R_P
	| atomic-formula_term
	| L_P assign-op f-head f-exp // require :numeric-fluents
	| L_P ASSIGN function-term term R_P // require :object-fluents
	| L_P ASSIGN function-term UNDEFINED R_P // require :object-fluents
	;

/* <cond-effect> */

cond-effect:
	L_P AND list_p-effect R_P
	| p-effect
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
	L_P DURATIVE_ACTION da-symbol PARAMETERS L_P typed-list_variable R_P da-def-body R_P
	;

/* <da-symbol> */

da-symbol:
	name
	;

/* <da-def body> */

da-def-body:
	DURATION duration-constraint CONDITION emptyOr_da-GD EFFECT emptyOr_da-effect
	;

/* <da-GD> */

da-GD:
	pref-timed-GD
	| L_P AND list_da-GD R_P
	| L_P FORALL L_P typed-list_variable R_P da-GD R_P // require :universal-preconditions
	;

/* <pref-timed-GD> */

pref-timed-GD:
	timed-GD
	| L_P PREFERENCE pref-name_option1 // require :preferences
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
	L_P AND nvList_simple-duration-constraint R_P // require :duration-inequalities
	| L_P R_P
	| simple-duration-constraint
	;

/* <simple-duration-constraint> */

simple-duration-constraint:
	L_P d-op Q_M_DURATION d-value R_P
	| L_P AT time-specifier simple-duration-constraint R_P
	;

/* <d-op> */

d-op:
	LOE // require :duration-inequalities
	| GOE // require :duration-inequalities
	| EQ
	;

/* <d-value> */

d-value:
	NUMBER
	| f-exp // require :numeric-fluents
	;

/* <da-effect> */

da-effect:
	L_P AND list_da-effect R_P
	| timed-effect
	| L_P FORALL L_P typed-list_variable R_P da-effect R_P // require :conditional-effects
	| L_P WHEN da-GD timed-effect R_P // require :conditional-effects
	;

list_da-effect:
	/* empty */
	| da-effect list_da-effect
	;

/* <timed-effect> */

timed-effect:
	L_P AT time-specifier cond-effect R_P
	| L_P AT time-specifier f-assign-da R_P // require :numeric-fluents
	| L_P assign-op-t f-head f-exp-t R_P  // require :continuous-effects + :numeric-fluents
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
	| Q_M_DURATION  // require :duration-inequalities
	| f-exp
	;

/* <assign-op-t> */

assign-op-t:
	INCREASE
	| DECREASE
	;

/* <f-exp-t> */

f-exp-t:
	L_P STAR f-exp NS_T R_P
	| L_P STAR NS_T f-exp R_P
	| NS_T
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
	| init goal constraints-problem problem_option4 // require :constraints
	;

problem_option4:
	problem_option5
	| metric-spec problem_option5 // require :numeric-fluents
	;

problem_option5:
	R_P
	| length-spec R_P

/* <object declaration> */

object-declaration:
	L_P OBJECTS typed-list_name R_P
	;

/* <init> */

init:
	L_P INIT list_init-el R_P
	;

/* <init-el> */

init-el:
	literal_name
	| L_P AT number literal_name R_P // require :timed-initial-literals
	| L_P EQ basic-function-term number R_P // require :numeric-fluents
	| L_P EQ basic-function-term name R_P // require :object-fluents
	;

/* <basic-function-term> */

basic-function-term:
	function-symbol
	| L_P function-symbol list_name R_P
	;

/* <goal> */

goal:
	L_P GOAL pre-GD
	;

/* <contraints> */

constraints-problem:
	L_P CONSTRAINTS pref-con-GD R_P // require :constraints
	;

/* <pref-con-GD> */

pref-con-GD:
	L_P AND list_pref-con-GD R_P
	| L_P FORALL L_P typed-list_variable R_P pref-con-GD R_P // require :universal-preconditions
	| L_P PREFERENCE pref-con-GD_option1 // require :preferences
	| con-GD
	;

pref-con-GD_option1:
	con-GD R_P
	| pref-name con-GD R_P
	;

/* <con-GD> */

con-GD:
	L_P AND list_con-GD R_P
	| L_P FORALL L_P typed-list_variable R_P con-GD R_P
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

/* <con2-GD> */

con2-GD:
	con-GD
	| GD
	;

/* <metric-spec> */

metric-spec:
	L_P METRIC optimization metric-f-exp R_P // require :numeric-fluents
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
	| L_P IS_VIOLATED pref-name R_P // require :preferences
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

/* <integer> */

integer:
	NUM
	;


/***************************************************/
/*************** Temporally-Extended ***************/
/***************************************************/


/* <te-durative-action-def> */

te-durative-action-def:
	L_P DURATIVE_ACTION da-symbol PARAMETERS L_P typed-list_variable R_P te-da-def-body R_P
	;

/* <te-da-def body> */

te-da-def-body:
	DURATION duration-constraint TIMEPOINTS L_P START END list_te-time-specifier R_P TIMEALIASES L_P ALL L_B START END L_B list_te-time-interval R_P TIMECONSTRAINTS L_P L_P EQ L_P MINUS END START R_P Q_M_DURATION R_P list_te-time-constraints R_P CONDITION emptyOr_da-GD EFFECT emptyOr_da-effect
	;

/* <te-time-specifier> */

te-time-specifier:
	time-specifier
	| name // allow to define flypoints
	;

/* <te-time-interval> */

te-time-interval:
	interval L_B te-time-specifier te-time-specifier L_B
	;

/* <te-time-constraints> */

te-time-constraints:
	L_P binary-comp te-time-specifier te-time-specifier R_P // do we need to do operations on flypoints ?
	;


/***************************************************/
/****************** Miscellaneous ******************/
/***************************************************/

/* <function typed list (x)> */

function-typed-list_atomic-function-skeleton:
	/* empty */
	| nvList_atomic-function-skeleton MINUS function-type function-typed-list_atomic-function-skeleton
	| nvList_atomic-function-skeleton // require :numeric-fluents
	;


/* <typed list (x)> */

typed-list_name:
	list_name
	| nvList_name MINUS type typed-list_name // require :typing
	;

typed-list_variable:
	list_variable
	| nvList_variable MINUS type typed-list_variable // require :typing
	;

/* <emptyOr (x)> */

emptyOr_pre-GD:
	L_P R_P
	| pre-GD
	;

emptyOr_effect:
	L_P R_P
	| effect
	;

emptyOr_da-GD:
	L_P R_P
	| da-GD
	;

emptyOr_da-effect:
	L_P R_P
	| da-effect
	;

/* <litteral (t)> */

literal_term:
	atomic-formula_term
	| L_P NOT atomic-formula_term R_P
	;

literal_name:
	atomic-formula_name
	| L_P NOT atomic-formula_name R_P
	;

/* <atomic formula (t)> */

atomic-formula_term:
	L_P predicate list_term R_P
	| L_P EQ term term R_P // require :equality
	;

atomic-formula_name:
	L_P predicate list_name R_P
	| L_P EQ name name R_P // require :equality
	;

/* nvList_A (for non void list) -> A+ */

nvList_require-key:
	require-key list_require-key
	;

nvList_name:
	name list_name
	;

nvList_atomic-formula-skeleton:
	atomic-formula-skeleton list_atomic-formula-skeleton
	;

nvList_variable:
	variable list_variable
	;
	
nvList_atomic-function-skeleton:
	atomic-function-skeleton list_atomic-function-skeleton
	;

nvList_primitive-type:
	primitive-type list_primitive-type
	;

nvList_f-exp:
	f-exp list_f-exp
	;

nvList_digit:
	digit list_digit
	;

nvList_te-time-specifier:
	te-time-specifier list_te-time-specifier
	;

nvList_te-time-interval:
	te-time-interval list_te-time-interval
	;

nvList_simple-duration-constraint:
	simple-duration-constraint list_simple-duration-constraint
	;

nvList_f-exp-da:
	f-exp-da list_f-exp-da
	;

nvList_metric-f-exp:
	metric-f-exp list_metric-f-exp
	;

/* list_A -> A* */

list_structure-def:
	/* empty */
	| structure-def list_structure-def
	;

list_require-key:
	/* empty */
	| require-key list_require-key
	;

list_name:
	/* empty */
	| name list_name
	;

list_atomic-formula-skeleton:
	/* empty */		
	| atomic-formula-skeleton list_atomic-formula-skeleton
	;

list_variable:
	/* empty */
	| variable list_variable
	;

list_atomic-function-skeleton:
	/* empty */
	| atomic-function-skeleton list_atomic-function-skeleton
	;

list_primitive-type:
	/* empty */
	| primitive-type list_primitive-type
	;

list_pre-GD:
	pre-GD list_pre-GD
	;

list_term:
	/* empty */
	| term list_term
	;

list_GD:
	/* empty */
	| GD list_GD
	;

list_f-exp:
	/* empty */
	| f-exp list_f-exp
	;

list_any-char:
	/* empty */
	| any-char list_any-char
	;

list_digit:
	/* empty */
	| digit list_digit
	;

list_c-effect:
	/* empty */
	| c-effect list_c-effect
	;

list_p-effect:
	/* empty */
	| p-effect list_p-effect
	;

list_te-time-specifier:
	/* empty */
	| te-time-specifier list_te-time-specifier
	;

list_te-time-interval:
	/* empty */
	| te-time-interval list_te-time-interval
	;

list_te-time-constraints:
	/* empty */
	| te-time-constraints list_te-time-constraints
	;

list_da-GD:
	/* empty */
	| da-GD list_da-GD
	;

list_simple-duration-constraint:
	/* empty */
	| simple-duration-constraint list_simple-duration-constraint
	;

list_f-exp-da:
	/* empty */
	| f-exp-da list_f-exp-da
	;

list_init-el:
	/* empty */
	| init-el list_init-el
	;

list_pref-con-GD:
	/* empty */
	| pref-con-GD list_pref-con-GD
	;

list_con-GD:
	/* empty */
	| con-GD list_con-GD
	;

list_metric-f-exp:
	/* empty */
	| metric-f-exp list_metric-f-exp
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

