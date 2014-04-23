#include "parser/Parser.h"

using namespace std;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "src/graph.h"

int main(int argc, char **argv)
{
	
    Parser parser_domain;
	FILE *domain_file, *problem_file;
	Data * data ;
	
	Graph *graph;
	Domain * domain;
	Problem * problem;

	if (argc != 3) {
		cerr << "Usage :\n\t" << argv[0] << " domainFile.pddl problemFile.pddl" << endl;
		exit(1);
	}
	
	if ((domain_file = fopen(argv[1], "r")) == NULL) {
		cerr << argv[1] << " cannot be opened." << endl;
		exit(2);
	}
	
	if ((problem_file = fopen(argv[2], "r")) == NULL) {
		cerr << argv[2] << " cannot be opened." << endl;
		exit(3);
	}
	
	dup2(fileno(domain_file), STDIN_FILENO);
	
	if (fclose(domain_file) != 0) {
		cerr << argv[1] << " cannot be closed." << endl;
		exit(4);
	}
	
	parser_domain.init();
	if (parser_domain.parse() == 0) {
		cout << "The domain was successfully parsed" << endl;
		Parser parser_problem;
		
		parser_problem.setData(&parser_domain);
		dup2(fileno(problem_file), STDIN_FILENO);
		
		if (fclose(problem_file) != 0) {
			cerr << argv[2] << " cannot be closed." << endl;
			exit(5);
		}
		
		if (parser_problem.parse() == 0) {
			cout << "The problem was successfully parsed" << endl;
			data = parser_problem.getData();
		}
	}
	else {
		if (fclose(problem_file) != 0) {
			cerr << argv[2] << " cannot be closed." << endl;
			exit(5);
		}
	}
	//data->display();
	domain = data->getDomain();
	problem = data->getProblem();

	graph = new Graph(domain, problem);
	graph->generateGraph();


	return 0;
}
