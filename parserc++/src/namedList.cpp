#include "namedList.h"


NamedList::NamedList() {}

NamedList::NamedList(string * name, vector<string*> * list):m_name(name), m_list(list) {}

NamedList::~NamedList() {}

string * NamedList::get_Name() {
	return m_name;
}

vector<string*> * NamedList::get_List() {
	return m_list;
}

string NamedList::to_string() {
	string str = "Name : " + *m_name + " - List : ";
	for (vector<string*>::iterator it = m_list->begin(); it != m_list->end(); ++it) {
		str += *(*it) + " ";
	}
	return str;
}

