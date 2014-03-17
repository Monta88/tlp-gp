#include "fluent.h"

#include <algorithm>

Fluent::Fluent(string * name, std::vector<NamedList*> * named_list):m_name(*name) {
	m_types = vector<Type *> ();
	
	for (vector<NamedList*>::iterator it = named_list->begin(); it != named_list->end(); ++it) {
		for (int i=0; i<(*it)->get_List()->size(); ++i) {
			// store the type each time there is a variable of this type
			Type * type = new Type(*(*it)->get_Name());
			m_types.push_back(type);
		}
	}
}

Fluent::~Fluent() {}

string Fluent::get_Name() {
	return m_name;
}

vector<Type *> Fluent::get_Types() {
	return m_types;
}

string Fluent::to_string() {
	string str = "Predicate " + m_name + " :";
	for(vector<Type *>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
		str += " " + (*it)->to_string();
	}
	return str;
}

