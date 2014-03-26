#include "domain.h"

Domain::Domain(string name):m_name(name) {
	m_constants = vector<Constant*> ();
	m_types = vector<Type*> (); 
	m_predicates = vector<Fluent*> (); 
	m_actions = vector<Action*> (); 
}

Domain::Domain(){}

Domain::~Domain() {
	m_constants.~vector();
	m_types.~vector();
	m_predicates.~vector();
	m_actions.~vector();
}

string Domain::getName() {
	return m_name;
}

void Domain::addType(Type * type)
{
	m_types.push_back(type);
}

void Domain::addPredicate(Fluent * predicate)
{
	m_predicates.push_back(predicate);
}

void Domain::addDurativeAction(Action * action)
{
	m_actions.push_back(action);
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
	for (vector<Action *>::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	return str;
}

vector<string> Domain::listNameAction(){
	vector<string> ret;
	for (vector<Action*>::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {	
			ret.push_back((*it)->getName());
	}
	return ret;
}
