#include "constant.h"


Constant::Constant(string name, vector<Type*> types):m_name(name), m_types(types) {}

Constant::Constant() {}

Constant::~Constant() {}

string Constant::getName() {
	return m_name;
}

vector<Type*> * Constant::getTypes() {
	return &m_types;
}

string Constant::to_string() {
	string str = "Constant " + m_name + " - ";
	if (m_types.size() > 0) {
		if (m_types.size() == 1) {
			str += m_types.at(0)->getName();
		}
		else {
			str += "(either";
			for (vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
				str += " " + (*it)->getName();
			}
			str += ")";
		}
	}
	return str;
}

