/*!
 * \file type.cpp
 * \brief This class represent a PDDL type
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#include "type.h"
#include <iostream>

Type::Type(string name, vector<Type*> parents):m_name(name), m_parents(parents) {}

Type::Type(string name):m_name(name) {
	m_parents = vector<Type*> ();
}

Type::Type() {}

Type::~Type() {}

void Type::addParents(vector<Type*> parents) {
	// for each type if the either type in types
	for (vector<Type*>::iterator it_parents = parents.begin(); it_parents != parents.end(); ++it_parents) {
		bool contains = false; // tell if the current type is already a type of the object
		
		// we verify it is not in the either type m_types of the object
		for (vector<Type*>::iterator it_m_parents = m_parents.begin(); (!contains) && (it_m_parents != m_parents.end()); ++it_m_parents) {
			if ((*it_m_parents)->getName() == (*it_parents)->getName()) { // if the types are the same
				contains = true;
			}
		}
		
		if (!contains) { // if the objects didn't have this type yet
			// we add this type
			m_parents.push_back(*it_parents);
		}
	}
}

string Type::getName() {
	return m_name;
}

vector<Type*> * Type::getParents() {
	return &m_parents;
}

bool Type::isOneOfParents(string name) {
	// for each type of the either type of the parent
	for (vector<Type*>::iterator it = m_parents.begin(); it != m_parents.end(); ++it) {
		// we check if the current type is the right one, or if it is one of its parents
		if (((*it)->getName() == name)||((*it)->isOneOfParents(name))) {
			return true;
		}
	}
	return false;
}

string Type::to_string() {
	string str = "Type " + m_name;
	if (m_parents.size() > 0) { // if it has a type
		if (m_parents.size() == 1) { // if it is a simple type
			str += " (Parent " + m_parents.at(0)->getName() + ")";
		}
		else { // if it is an either type
			str += " (Parents either";
			
			// we print all the types of the either type
			for (vector<Type*>::iterator it = m_parents.begin(); it != m_parents.end(); ++it) {
				str += " " + (*it)->getName();
			}
			str += ")";
		}
	}
	return str;
}

