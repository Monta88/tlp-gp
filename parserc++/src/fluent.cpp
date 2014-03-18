#include "fluent.h"

#include <algorithm>

Fluent::Fluent(string * name, std::vector<TypedList*> * typed_list):m_name(*name) {
	/*m_types = vector<Type *> ();
	
	for (vector<TypedList*>::iterator it = typed_list->begin(); it != typed_list->end(); ++it) {
		for (vector<string*>::iterator it_type = (*it)->get_List()->begin(); it_type != (*it)->get_List()->end(); ++it_type) {
			// store the type each time there is a variable of this type
			Type * type = new Type(*(*it)->get_Name()->at(0));
			m_types.push_back(type);
		}
	}*/
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

