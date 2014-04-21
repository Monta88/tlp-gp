#include "member.h"


Member::Member(string name, vector<Type*> types):m_name(name), m_types(types) {}

Member::Member() {}

Member::~Member() {}

string Member::getName() {
	return m_name;
}

vector<Type*> * Member::getTypes() {
	return &m_types;
}

string Member::to_string() {
	string str = "Member " + m_name + " - ";
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

void Member::changeName(string name){
	m_name=name;
}
