#include "parser/Parser.h"

using namespace std;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

int main(int argc, char **argv)
{
    Parser parser;
    
	FILE * file;
	if (argc != 2) {
		cerr << "Usage :\n\t" << argv[0] << " PDDL_File" << endl;
		exit(1);
	}
	
	if ((file = fopen(argv[1], "r")) == NULL) {
		cerr << argv[1] << " cannot be opened." << endl;
		exit(2);
	}
	
	dup2(fileno(file), STDIN_FILENO);
	
	if (fclose(file) != 0) {
		cerr << argv[1] << " cannot be closed." << endl;
		exit(3);
	}
	
	parser.init();

	if (parser.parse() == 0)
		cout << "\nParsing successful" << endl;

	parser.display();

	return 0;
}
