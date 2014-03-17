/*
 * Data.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#include "Data.h"
#include "src/constant.h"
#include "src/type.h"

#include <algorithm>

Data::Data() {
	m_requirements = vector<int> ();
	m_constants = vector<Constant *> ();
}

Data::~Data() {}

void Data::add_Domain(string str) {
	m_domain = new Domain(str);
}

bool Data::add_Constants(vector<NamedList*> * named_list) {
	for (vector<NamedList*>::iterator it = named_list->begin(); it != named_list->end(); ++it) {
		vector<string*> * list = (*it)->get_List();
		for (vector<string*>::iterator it_bis = list->begin(); it_bis != list->end(); ++it_bis) {
			Constant * constant = new Constant(*(*it_bis), new Type(*(*it)->get_Name()));
			if (is_Constant(constant))
				return false;
			m_constants.push_back(constant);
		}
	}
	return true;
}

bool Data::add_Requirement(int req) {
	if (is_Requirement(req))
		return false;
	m_requirements.push_back(req);
	return true;
}

bool Data::is_Constant(Constant * constant) {
	return (find(m_constants.begin(), m_constants.end(), constant) != m_constants.end());
}

bool Data::is_Requirement(int req) {
	return (find(m_requirements.begin(), m_requirements.end(), req) != m_requirements.end());
}

void Data::display() {
	cout << "Domain name : " << m_domain->getName() << endl;
	
	cout << "Constants : " << endl;
	for(int i=0; i<m_constants.size(); i++)
		cout << i << ": " << m_constants[i]->to_string() << endl;
	
	cout << "End of display." << endl;
	
}

