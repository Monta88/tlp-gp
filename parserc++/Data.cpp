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
	m_types = new vector<Type *> ();
	m_constants = new vector<Constant *> ();
	m_predicates = new vector<Fluent *> ();
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

bool Data::add_Constants(vector<NamedList*> * named_list) {
	reverse(named_list->begin(), named_list->end()); // the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<NamedList*>::iterator it = named_list->begin(); it != named_list->end(); ++it) {
		reverse((*it)->get_Name()->begin(), (*it)->get_Name()->end());
		// for each type in list_name returned in type construct the named_list with this type
		for (vector<string*>::iterator it_type = (*it)->get_Name()->begin(); it_type != (*it)->get_Name()->end(); ++it_type) {
			reverse((*it)->get_List()->begin(), (*it)->get_List()->end());
			for (vector<string*>::iterator it_const = (*it)->get_List()->begin(); it_const != (*it)->get_List()->end(); ++it_const) {
				Constant * constant = new Constant(*(*it_const), new Type(*(*it_type)));
				if (is_Constant(constant)) {
					delete constant;
					return false;
				}
				m_constants->push_back(constant);
			}
		}
	}
	return true;
}

bool Data::is_Constant(Constant * constant) {
	return (find(m_constants->begin(), m_constants->end(), constant) != m_constants->end());
}

bool Data::add_Predicate(std::string * name, std::vector<NamedList*> * named_list) {
	reverse(named_list->begin(), named_list->end()); // the parser is recursive so we must reverse in order to have the arguments of the predicate in the same order than in the file
	Fluent * predicate = new Fluent(name, named_list);
	if (is_Predicate(predicate)) {
		delete predicate;
		return false;
	}
	m_predicates->push_back(predicate);
	return true;
}

bool Data::is_Predicate(Fluent * predicate) {
	return (find(m_predicates->begin(), m_predicates->end(), predicate) != m_predicates->end());
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

