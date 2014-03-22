#include "domain.h"

Domain::Domain(string name):m_name(name) {
	m_constants = vector<Constant*> ();
	m_types = vector<Type*> (); 
	m_predicates = vector<Fluent*> (); 
	m_actions = vector<Action*> (); 
}

Domain::Domain() {}

Domain::~Domain() {}

string Domain::getName() {
	return m_name;
}

string Domain::to_string() {
	string str;
	str = "Domain : " + m_name;
	str += "\nConstants :";
	for (vector<Constant*>::iterator it = m_constants.begin(); it != m_constants.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nTypes :";
	for (vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nPredicates :";
	for (vector<Fluent*>::iterator it = m_predicates.begin(); it != m_predicates.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nActions :";
	for (vector<Action*>::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	return str;
}

