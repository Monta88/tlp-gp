#include "typedList.h"


TypedList::TypedList() {}

TypedList::TypedList(vector<string> * types, vector<string> * list):m_types(*types), m_list(*list) {}

TypedList::~TypedList() {}

vector<string> * TypedList::getTypes() {
	return &m_types;
}

vector<string> * TypedList::getList() {
	return &m_list;
}

string TypedList::to_string() {
	string str = "Typed List : ";
	if (m_types.size() > 0) {
		if (m_types.size() == 1) {
			str += m_types.at(0);
		}
		else {
			str += "either";
			for (vector<string>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
				str += " " + (*it);
			}
		}
	}
	str += " -";
	for (vector<string>::iterator it = m_list.begin(); it != m_list.end(); ++it) {
		str += " " + (*it);
	}
	return str;
}

