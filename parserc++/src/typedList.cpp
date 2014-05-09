/*!
 * \file typedList.cpp
 * \brief This class represent a parser's typedList, which is a pair of a list of name and an either type
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#include "typedList.h"


TypedList::TypedList() {}

TypedList::TypedList(vector<string> * types, vector<string> * list):m_types(*types), m_list(*list) {}

TypedList::~TypedList() {}

vector<string> * TypedList::getTypes() {
	return &m_types;
}

vector<string> * TypedList::getList() {
	return &m_list;
}

string TypedList::to_string() {
	string str = "Typed List : ";
	if (m_types.size() > 0) { // if it has a type
		if (m_types.size() == 1) { // if it is a simple type
			str += m_types.at(0);
		}
		else { // if it is an either type
			str += "either";
			
			// we print all the types of the either type
			for (vector<string>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
				str += " " + (*it);
			}
		}
	}
	
	str += " -";
	
	// for each name in the list m_list
	for (vector<string>::iterator it = m_list.begin(); it != m_list.end(); ++it) {
		str += " " + (*it);
	}
	return str;
}

