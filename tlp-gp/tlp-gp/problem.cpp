#include "problem.h"


Problem::Problem(string name,Domain *domain):name(name),domain(domain)
{
}


Problem::~Problem()
{
}

void Problem::add_Object(Variable *var)
{
	objects.push_back(var);
}

void Problem::add_Init(Fluent *fluent,vector<Variable *> vars)
{
	inits.push_back(make_pair(fluent,vars));
}

void Problem::add_Goal(Fluent *fluent,vector<Variable *> vars)
{
	goals.push_back(make_pair(fluent,vars));
}
