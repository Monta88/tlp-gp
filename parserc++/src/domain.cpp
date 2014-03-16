#include "domain.h"


Domain::Domain(string name):m_name(name) {}


Domain::~Domain() {
	m_types.~vector();
	m_predicates.~vector();
	m_actions.~vector();
}

string Domain::getName() {
	return m_name;
}

void Domain::add_Type(Type * type) {
	m_types.push_back(type);
}

void Domain::add_Predicate(Fluent * predicate) {
	m_predicates.push_back(predicate);
}

void Domain::add_Action(Action * action) {
	m_actions.push_back(action);
}

string Domain::to_string() {
	string rep;
	rep = "Domain name" + m_name + " \n types : ";
	/*for (vector<Type *>::const_iterator it = m_types.begin(); it != m_types.end(); it++)
	{	
		rep +=(*it)->get_Name() + " , ";
	}
	/*rep += "\n Predicates : ";
	for (unsigned int i=0; i<m_predicates.size(); ++i)
	{	
		rep += m_predicates[i]->get_Name() + " , ";
	}
	/*rep += "\n Actions : ";
	for (unsigned int i=0; i<m_actions.size(); ++i)
	{	
		rep += m_actions[i].get_Name() + " , ";
	}*/
	return rep;
}

