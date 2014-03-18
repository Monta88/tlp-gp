#include "type.h"

Type::Type(Type * parent, vector<string*> * name_list):m_parent(parent), m_name_list(name_list) {}

Type::Type(vector<string*> * parent, string * name) {
	m_parent = new Type(parent);
	m_name_list = new vector<string*> ();
	m_name_list->push_back(name);
}

Type::Type(vector<string*> * name_list):m_name_list(name_list) {
	m_parent = NULL;
}

Type::Type() {} 

Type::~Type() {} 
     
vector<string*> * Type::get_Name_List() {
	return m_name_list;
}

Type * Type::get_Parent() {
	return m_parent;
}

string Type::to_string() {
	string str;
	if (m_parent == NULL) {
		str = "Type (Parent NULL)";
	}
	else {
		str = "Type (Parent " + m_parent->to_string() + ")";
	}
	if (m_name_list->size() == 1) {
		str += " " + *(m_name_list->at(0));
	}
	else {
		str += "either";
		for (vector<string*>::iterator it = m_name_list->begin(); it != m_name_list->end(); ++it) {
			str += " " + *(*it);
		}
	}
	return str;
}

