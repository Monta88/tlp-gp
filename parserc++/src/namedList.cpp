#include "namedList.h"


NamedList::NamedList() {}

NamedList::NamedList(vector<string*> * name, vector<string*> * list):m_name(name), m_list(list) {}

NamedList::~NamedList() {}

vector<string*> * NamedList::get_Name() {
	return m_name;
}

vector<string*> * NamedList::get_List() {
	return m_list;
}

string NamedList::to_string() {
	string str = "Named List : ";
	for (int i=0; i<m_name->size(); ++i) {
		str += (i+1) + " : " + *m_name->at(i) + " - ";
		for (vector<string*>::iterator it = m_list->begin(); it != m_list->end(); ++it) {
			str += *(*it) + " ";
		}
		str += " || ";
	}
	return str;
}

