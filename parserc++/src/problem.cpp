#include "problem.h"


Problem::Problem(string name):m_name(name) {
		m_objects = new vector<Object*> ();
		m_inits = new vector<pair<Fluent*, Attribute> > ();
		m_goals = new vector<pair<Fluent*, Attribute> > ();
}

Problem::Problem() {}

Problem::~Problem() {}

string Problem::getName() {
	return m_name;
}

void Problem::addObjects(vector<Object*> * objects) {
	m_objects = objects;
}

void Problem::addInits(vector<pair<Fluent*, Attribute> > * inits) {
	m_inits = inits;
}

void Problem::addGoals(vector<pair<Fluent*, Attribute> > * goals) {
	m_goals = goals;
}

