
#ifndef PROBLEM_H
#define PROBLEM_H
#include <iostream> 
#include <string> 
#include "domain.h"
#include <vector>
#include "variable.h"
#include "fluent.h"

using namespace std;

class Problem
{
public:
	Problem(string name,Domain *domain);
	void add_Object(Variable *);
	void add_Init(Fluent *fluent,vector<Variable *> vars);
	void add_Goal(Fluent *fluent,vector<Variable *> vars);
	~Problem();
private:
	string name;
	Domain * domain;
	vector<Variable *> objects;
	vector< pair< Fluent *,vector<Variable *> > >  inits;
	vector< pair< Fluent *,vector<Variable *> > >  goals;
};

#endif // PROBLEM_H
