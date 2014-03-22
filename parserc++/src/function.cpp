#include "function.h"


Function::Function(string name, vector<Type*> return_type, vector< vector<Type*> > types_list):m_name(name), m_return_type(return_type), m_types_list(types_list) {}

Function::~Function() {}

string Function::getName() {
	return m_name;
}

vector<Type*> * Function::getReturnType() {
	return &m_return_type;
}

vector< vector<Type*> > * Function::getTypesList() {
	return &m_types_list;
}

string Function::to_string() {
	string str = "Function " + m_name + " :";
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
	if (m_return_type.size() == 1) {
		str += " - " + m_return_type.at(0)->getName();
	}
	else {
		if (m_return_type.size() == 0) {
			str += " - void";
		}
		else {
			str += " - (either";
			for (vector<Type*>::iterator it_either = m_return_type.begin(); it_either != m_return_type.end(); ++it_either) {
				str += " " + (*it_either)->getName();
			}
			str += ")";
		}
	}
	return str;
}

