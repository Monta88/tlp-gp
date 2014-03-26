#include "object.h"


Object::Object(string name, vector<Type*> types):m_name(name), m_types(types) {}

Object::Object() {}

Object::~Object() {}

string Object::getName() {
	return m_name;
}

vector<Type*> * Object::getTypes() {
	return &m_types;
}

string Object::to_string() {
	string str = "Object " + m_name + " - ";
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

