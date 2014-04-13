#include "domain.h"

Domain::Domain(string name):m_name(name) {
	m_types = new vector<Type*> ();
	m_constants = new vector<Constant*> ();
	m_predicates = new vector<Predicate*> ();
	m_functions = new vector<Function*> ();
	m_actions = new vector<DurativeAction*> ();
}

Domain::Domain() {}

Domain::~Domain() {}

string Domain::getName() {
	return m_name;
}

void Domain::addTypes(vector<Type*> * types)
{
	m_types = types;
}

void Domain::addConstants(vector<Constant*> * constants)
{
	m_constants = constants;
}

void Domain::addPredicates(vector<Predicate*> * predicates)
{
	m_predicates = predicates;
}

void Domain::addFunctions(vector<Function*> * functions)
{
	m_functions = functions;
}

void Domain::addActions(vector<DurativeAction*> * actions)
{	
	m_actions = actions;	
}

string Domain::to_string() {
	string str="";
	str = "Domain : " + m_name;
	str += "\nTypes :";
	for (vector<Type*>::iterator it = m_types->begin(); it != m_types->end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nConstants :";
	for (vector<Constant*>::iterator it = m_constants->begin(); it != m_constants->end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nPredicates :";
	for (vector<Predicate*>::iterator it = m_predicates->begin(); it != m_predicates->end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nFunctions :";
	for (vector<Function*>::iterator it = m_functions->begin(); it != m_functions->end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "Action : ";
	for (vector<DurativeAction*>::iterator it = m_actions->begin(); it != m_actions->end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	return str;
}

vector<string> Domain::listNameAction(){
	vector<string> ret;
	for (vector<DurativeAction *>::iterator it = m_actions->begin(); it != m_actions->end(); ++it) {	
			ret.push_back((*it)->getName());
	}
	return ret;
}

vector<DurativeAction *> * Domain::getActions(){
	return m_actions;
}
