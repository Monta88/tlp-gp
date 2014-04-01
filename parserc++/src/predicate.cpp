#include "predicate.h"

Predicate::Predicate(string name):m_name(name){}

Predicate::Predicate(string name, vector< vector<Type*> > types_list):m_name(name), m_types_list(types_list) {}

Predicate::~Predicate() {}

string Predicate::getName() {
	return m_name;
}

vector< vector<Type*> > * Predicate::getTypesList() {
	return &m_types_list;
}


string Predicate::to_string() {
	string str = "Predicate " + m_name + " :";
	for(vector< vector<Type*> >::iterator it = m_types_list.begin(); it != m_types_list.end(); ++it) {
		if ((*it).size() > 0) {
			if ((*it).size() == 1) {
				str += " " + (*it).at(0)->getName();
			}
			else {
				str += " (either";
				for (vector<Type*>::iterator it_either = (*it).begin(); it_either != (*it).end(); ++it_either) {
					str += " " + (*it_either)->getName();
				}
				str += ")";
			}
		}
	}
	return str;
}

