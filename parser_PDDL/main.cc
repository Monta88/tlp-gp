/* main.cc */

#include "heading.h"

// prototype of bison-generated parser function
int yyparse();

int main(int argc, char **argv)
{
	FILE * file;
	if (argc != 2) {
		cerr << "Usage :\n\t" << argv[0] << " PDDL_File\n";
		exit(1);
	}
	
	if ((file = fopen(argv[1], "r")) == NULL) {
		cerr << argv[1] << " cannot be opened.\n";
		exit(2);
	}
	
	dup2(fileno(file), STDIN_FILENO);
	
	if (yyparse() == 0)
		printf("Parsing surccessful\n");

	fclose(file);

	return 0;
}

