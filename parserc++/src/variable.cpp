#include "variable.h"


Variable::Variable(string name, vector<Type*> types): Member(name, types) {}

Variable::Variable(): Member() {}

string Variable::to_string() {
	string str = "Variable " + m_name + " - ";
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

string Variable::getClass(){
	return "Variable";
}
