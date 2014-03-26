#include "fluent.h"


Fluent::Fluent(string name, vector< vector<Type*> > types_list):m_name(name), m_types_list(types_list) {}

Fluent::~Fluent() {}

string Fluent::getName() {
	return m_name;
}

vector< vector<Type*> > * Fluent::getTypesList() {
	return &m_types_list;
}


string Fluent::to_string() {
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

