#include "typedList.h"


TypedList::TypedList() {}

TypedList::TypedList(vector<string*> * type, vector<string*> * list):m_type(type), m_list(list) {}

TypedList::~TypedList() {}

vector<string*> * TypedList::get_Type() {
	return m_type;
}

vector<string*> * TypedList::get_List() {
	return m_list;
}

string TypedList::to_string() {
	string str = "Typed List : "; 
	if (m_type->size() == 1) {
		str += *m_type->at(0);
	}
	else {
		str += "either";
		for (vector<string*>::iterator it = m_type->begin(); it != m_type->end(); ++it) {
			str += " " + *(*it);
		}
	}
	str += " -";
	for (vector<string*>::iterator it = m_list->begin(); it != m_list->end(); ++it) {
		str += " " + *(*it);
	}
	return str;
}

