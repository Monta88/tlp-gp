#include "domain.h"

Domain::Domain(string name):m_name(name) {
	m_constants = vector<Constant*> ();
	m_types = vector<Type*> (); 
	m_predicates = vector<Predicate*> (); 
	m_actions = vector<DurativeAction*> (); 
}

Domain::Domain(){}

Domain::~Domain() {}

string Domain::getName() {
	return m_name;
}

void Domain::addType(Type * type)
{
	m_types.push_back(type);
}

void Domain::addPredicate(Predicate * predicate)
{
	m_predicates.push_back(predicate);
}

void Domain::addDurativeAction(DurativeAction * action)
{	
	m_actions.push_back(action);	
}

string Domain::to_string() {
	string str="";
	/*str = "Domain : " + m_name;
	str += "\nConstants :";
	for (vector<Constant*>::iterator it = m_constants.begin(); it != m_constants.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nTypes :";
	for (vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}
	str += "\nPredicates :";
	for (vector<Predicate*>::iterator it = m_predicates.begin(); it != m_predicates.end(); ++it) {	
		str += "\n\t" + (*it)->to_string();
	}*/
	str += "Action : ";
	if (m_actions.size()>0){str += " plus de 1";} else { str +="aucune actions";}
	for (vector<DurativeAction *>::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {	
		str += "\n\t" + (*it)->to_string()+"";
	}
	return str;
}

vector<string> Domain::listNameAction(){
	vector<string> ret;
	for (vector<DurativeAction *>::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {	
			ret.push_back((*it)->getName());
	}
	return ret;
}

vector<DurativeAction *> Domain::getActions(){
	return m_actions;
}
