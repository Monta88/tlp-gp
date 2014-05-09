/*!
 * \file variable.cpp
 * \brief This class represent a PDDL variable, inherit from Member
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#include "variable.h"


Variable::Variable(string name, vector<Type*> types): Member(name, types) {}

Variable::Variable(): Member() {}

string Variable::to_string() {
	string str = "Variable " + m_name + " - ";
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

string Variable::getClass(){
	return "Variable";
}
