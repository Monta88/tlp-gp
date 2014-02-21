%{
#include <cstdio>
using namespace std;
#include <iostream>
#include "my.tab.h"  // to get the token types that we return

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
 
void yyerror(const char *s);
%}
%union {
	char *sval;
}

%token LB
%token RB
%token DEFINE
%token DOMAINE
%token PROBLEM
%token REQUIREMENTS
%token TYPES
%token CONSTANT
%token PREDICATES
%token FUNCTIONS
%token ACTION
%token DURATIVEACTION
%token <sval> NAME

%%
start:		
		LB define fin
	;
define : DEFINE idontknow
	;

idontknow : LB DOMAINE NAME fin dom
		| LB PROBLEM NAME fin prob

dom :	 REQUIREMENTS reql 
		|type
		;

reql : LB fin;

type:  TYPES typel
		|const;

typel: LB fin;

const:  CONSTANT constl
		|pred

constl: LB fin;

pred:  PREDICATES predl
		|fun
		;

predl: LB fin;

fun:  FUNCTIONS funl
	|action
	|durativeaction
	;

funl:LB fin;

action:  action  act
		|act;

act :  LB ACTION fin ;

durativeaction:  durativeaction dact
				|dact;

dact :  LB DURATIVEACTION fin;

prob : LB fin;

fin : 
		RB
	;
%%
main(int argc, char *argv[] ) {
	if (argc != 2)	
	{
		cout<<"Error no argument";
		return -1;
	}
	FILE *myfile = fopen(argv[1],"r");
	if (!myfile) {
		cout << "I can't open a.snazzle.file!" << endl;
		return -1;
	}
	yyin = myfile;

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
	
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}
