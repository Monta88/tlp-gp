#include "type.h"
#include <iostream>

Type::Type(string name, vector<Type*> parents):m_name(name), m_parents(parents) {}

Type::Type(string name):m_name(name) {
	m_parents = vector<Type*> ();
}

Type::Type() {}

Type::~Type() {}

void Type::addParents(vector<Type*> parents) {
	m_parents.insert(m_parents.end(), parents.begin(), parents.end());
}

string Type::getName() {
	return m_name;
}

vector<Type*> * Type::getParents() {
	return &m_parents;
}

bool Type::isOneOfParents(string name) {
	for (vector<Type*>::iterator it = m_parents.begin(); it != m_parents.end(); ++it) {
		if (((*it)->getName() == name)||((*it)->isOneOfParents(name))) {
			return true;
		}
	}
	return false;
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

