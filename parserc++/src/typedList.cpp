#include "typedList.h"


TypedList::TypedList() {}

TypedList::TypedList(string * type, vector<string*> * list_name):m_type(type), m_list_name(list_name) {}

TypedList::~TypedList() {}

string TypedList::to_string() {
	string str = "Type : " + *m_type + " - Names : ";
	for (vector<string*>::iterator it = m_list_name->begin(); it != m_list_name->end(); ++it) {
		str += *(*it) + " ";
	}
	return str;
}

