#include "problem.h"


Problem::Problem(string name,Domain * domain):m_name(name),m_domain(domain) {}

Problem::Problem() {}

Problem::~Problem() {}

string Problem::getName() {
	return m_name;
}

Domain * Problem::getDomain() {
	return m_domain;
}

/*
void Problem::add_Object(Variable *var) {
	m_objects.push_back(var);
}

void Problem::add_Init(Fluent *fluent,vector<Variable *> vars) {
	m_inits.push_back(make_pair(fluent,vars));
}

void Problem::add_Goal(Fluent *fluent,vector<Variable *> vars) {
	m_goals.push_back(make_pair(fluent,vars));
}*/

