/*
 * Data.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#include "Data.h"

#include <algorithm>
#include <iostream>

Data::Data() {
	m_requirements = new vector<int> ();
	m_type_list = new vector<string> ();
	m_constant_list = new vector<string> ();
	m_predicate_list = new vector<string> ();
	m_types = new vector<Type*> ();
	m_constants = new vector<Constant*> ();
	m_predicates = new vector<Fluent*> ();
}

Data::~Data() {}

void Data::add_Domain(string str) {
	m_domain = new Domain(str);
}

bool Data::add_Requirement(int req) {
	if (is_Requirement(req)) {
		return false;
	}
	m_requirements->push_back(req);
	return true;
}

bool Data::is_Requirement(int req) {
	return (find(m_requirements->begin(), m_requirements->end(), req) != m_requirements->end());
}

bool Data::add_Types(std::vector<TypedList*> * typedList_list) {
	reverse(typedList_list->begin(), typedList_list->end()); // the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		reverse((*it)->get_List()->begin(), (*it)->get_List()->end());
		for (vector<string*>::iterator it_type = (*it)->get_List()->begin(); it_type != (*it)->get_List()->end(); ++it_type) {
			if (is_Type(*it_type)) {
				return false;
			}
			m_type_list->push_back(*(*it_type));
			m_types->push_back(new Type((*it)->get_Type(), (*it_type)));
		}
	}
	return true;
}

bool Data::is_Type(string * type) {
	return (find(m_type_list->begin(), m_type_list->end(), *type) != m_type_list->end());
}

Type * Data::get_Type(vector<string*> * name_list) {
	// TODO when type interface is coded
	return m_types->at(0);
}

bool Data::add_Constants(vector<TypedList*> * typedList_list) {
	Type * type;
	reverse(typedList_list->begin(), typedList_list->end()); // the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		reverse((*it)->get_List()->begin(), (*it)->get_List()->end());
		for (vector<string*>::iterator it_constant = (*it)->get_List()->begin(); it_constant != (*it)->get_List()->end(); ++it_constant) {
			if (is_Constant(*it_constant)) {
				return false;
			}
			m_constant_list->push_back(*(*it_constant));
			if ((type = get_Type((*it)->get_Type())) == NULL) {
				return false;
			}
			m_constants->push_back(new Constant((*it_constant), type));
		}
	}
	return true;
}

bool Data::is_Constant(string * constant) {
	return (find(m_constant_list->begin(), m_constant_list->end(), *constant) != m_constant_list->end());
}

bool Data::add_Predicate(std::string * name, std::vector<TypedList*> * typedList_list) {
	/*reverse(typedList_list->begin(), typedList_list->end()); // the parser is recursive so we must reverse in order to have the arguments of the predicate in the same order than in the file
	Fluent * predicate = new Fluent(name, typedList_list);
	if (is_Predicate(predicate)) {
		delete predicate;
		return false;
	}
	m_predicates->push_back(predicate);*/
	return true;
}

bool Data::is_Predicate(string * predicate) {
	return (find(m_predicate_list->begin(), m_predicate_list->end(), *predicate) != m_predicate_list->end());
}

void Data::display() {
	cout << "Domain name : " << m_domain->getName() << endl;
	
	cout << "Types : " << endl;
	for(vector<Type *>::iterator it = m_types->begin(); it != m_types->end(); ++it)
		cout << "\t" << (*it)->to_string() << endl;
	
	cout << "Constants : " << endl;
	for(vector<Constant *>::iterator it = m_constants->begin(); it != m_constants->end(); ++it)
		cout << "\t" << (*it)->to_string() << endl;
	
	cout << "Predicates : " << endl;
	for(vector<Fluent *>::iterator it = m_predicates->begin(); it != m_predicates->end(); ++it)
		cout << "\t" << (*it)->to_string() << endl;
}

