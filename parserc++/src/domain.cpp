#include "domain.h"

#include <algorithm>

Domain::Domain(string name):m_name(name) {
	m_constants = vector<Constant *> ();
	m_types = vector<Type *> (); 
	m_predicates = vector<Fluent *> (); 
	m_actions = vector<Action *> (); 
}


Domain::~Domain() {
	m_constants.~vector();
	m_types.~vector();
	m_predicates.~vector();
	m_actions.~vector();
}

string Domain::getName() {
	return m_name;
}

bool Domain::add_Constant(Constant * constant) {
	if (!is_Constant(constant)) {
		return false;
	}
	else {
		m_constants.push_back(constant);
	}
	return true;
}

bool Domain::add_Type(Type * type) {
	if (!is_Type(type)) {
		return false;
	}
	else {
		m_types.push_back(type);
	}
	return true;
}

bool Domain::add_Predicate(Fluent * predicate) {
	if (!is_Predicate(predicate)) {
		return false;
	}
	else {
		m_predicates.push_back(predicate);
	}
	return true;
}

bool Domain::add_Action(Action * action) {
	if (!is_Action(action)) {
		return false;
	}
	else {
		m_actions.push_back(action);
	}
	return true;
}

bool Domain::is_Constant(Constant * constant) {
	return (find(m_constants.begin(), m_constants.end(), constant) != m_constants.end());
}

bool Domain::is_Type(Type * type) {
	return (find(m_types.begin(), m_types.end(), type) != m_types.end());
}

bool Domain::is_Predicate(Fluent * predicate) {
	return (find(m_predicates.begin(), m_predicates.end(), predicate) != m_predicates.end());
}

bool Domain::is_Action(Action * action) {
	return (find(m_actions.begin(), m_actions.end(), action) != m_actions.end());
}

string Domain::to_string() {
	string str;
	str = "Domain name :" + m_name + "\n";
	str += "\n Constants : ";
	for (vector<Constant *>::iterator it = m_constants.begin(); it != m_constants.end(); ++it) {	
		str += (*it)->get_Name() + ", ";
	}
	str += "\n Types : ";
	for (vector<Type *>::iterator it = m_types.begin(); it != m_types.end(); ++it) {	
		str += (*it)->get_Name() + ", ";
	}
	str += "\n Predicates : ";
	for (vector<Fluent *>::iterator it = m_predicates.begin(); it != m_predicates.end(); ++it) {	
		str += (*it)->get_Name() + ", ";
	}
	str += "\n Actions : ";
	for (vector<Action *>::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {	
		str += (*it)->get_Name() + ", ";
	}
	return str;
}

