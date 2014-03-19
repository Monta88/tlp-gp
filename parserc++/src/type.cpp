#include "type.h"


Type::Type(string name, vector<Type*> parents):m_name(name), m_parents(parents) {}

Type::Type(string name):m_name(name) {
	m_parents = vector<Type*> ();
}

Type::Type() {}

Type::~Type() {}

void Type::addParents(vector<Type*> parents) {
	m_parents = parents;
}

string Type::getName() {
	return m_name;
}

vector<Type*> * Type::getParents() {
	return &m_parents;
}

string Type::to_string() {
	string str = "Type " + m_name;
	if (m_parents.size() > 0) {
		if (m_parents.size() == 1) {
			str += " (Parent " + m_parents.at(0)->getName() + ")";
		}
		else {
			str += " (Parents either";
			for (vector<Type*>::iterator it = m_parents.begin(); it != m_parents.end(); ++it) {
				str += " " + (*it)->getName();
			}
			str += ")";
		}
	}
	return str;
}

