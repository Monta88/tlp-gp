/*!
 * \file member.cpp
 * \brief This class represent a PDDL constant, object or variable
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

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
	if (m_types.size() > 0) { // if it has a type
		if (m_types.size() == 1) { // if it is a simple type
			str += m_types.at(0)->getName();
		}
		else { // if it is an either type
			str += "(either";
			
			// we print all the types of the either type
			for (vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
				str += " " + (*it)->getName();
			}
			str += ")";
		}
	}
	return str;
}

string Member::getClass(){
	return "Member";
}

void Member::changeName(string name){
	m_name = name;
}

