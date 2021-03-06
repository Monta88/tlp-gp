/*!
 * \file object.h
 * \brief This class represent a PDDL object, inherit from Member
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#include "object.h"


Object::Object(string name, vector<Type*> types): Member(name, types) {}

Object::Object(): Member() {}

void Object::addTypes(vector<Type*> types) {
	// for each type if the either type in types
	for (vector<Type*>::iterator it_types = types.begin(); it_types != types.end(); ++it_types) {
		bool contains = false; // tell if the current type is already a type of the object
		
		// we verify it is not in the either type m_types of the object
		for (vector<Type*>::iterator it_m_types = m_types.begin(); (!contains) && (it_m_types != m_types.end()); ++it_m_types) {
			if ((*it_m_types)->getName() == (*it_types)->getName()) { // if the types are the same
				contains = true;
			}
		}
		
		if (!contains) { // if the objects didn't have this type yet
			// we add this type
			m_types.push_back(*it_types);
		}
	}
}

string Object::to_string() {
	string str = "Object " + m_name + " - ";
	if (m_types.size() > 0) { // if it has a type
		if (m_types.size() == 1) { // if it is a simple type
			str += m_types.at(0)->getName();
		}
		else { // if it is an either type
			str += "(either";
			
			// we print all the types of the either type
			for (vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
				str += " " + (*it)->getName();
			}
			str += ")";
		}
	}
	return str;
}

string Object::getClass(){
	return "Object";
}
