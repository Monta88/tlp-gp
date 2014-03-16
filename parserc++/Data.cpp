/*
 * Data.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#include "Data.h"
#include "src/constant.h"

#include <algorithm>

Data::Data() {
	m_data = vector<string> ();
	m_requirements = vector<int> ();
}

Data::~Data() {}

void Data::add(string str) {
	m_data.push_back(str);
}

void Data::add_Domain(string str) {
	m_domain = new Domain(str);
}

bool Data::add_Constant(string name) {
	if (!is_Constant(name)) {
		return false;
	}
	else {
		m_constants.push_back(new Constant(name));
	}
	return true;
}

bool Data::add_Constant(string name, string type) {
	if (!is_Constant(name, type)) {
		return false;
	}
	else {
		m_constants.push_back(new Constant(name, new Type(type)));
	}
	return true;
}

bool Data::add_Requirement(int req) {
	if (!is_Requirement(req)) {
		return false;
	}
	else {
		m_requirements.push_back(req);
	}
	return true;
}

bool Data::is_Constant(string name) {
	Constant * constant = new Constant(name);
	return (find(m_constants.begin(), m_constants.end(), constant) != m_constants.end());
}

bool Data::is_Constant(string name, string type) {
	Constant * constant = new Constant(name, new Type(type));
	return (find(m_constants.begin(), m_constants.end(), constant) != m_constants.end());
}

bool Data::is_Requirement(int req) {
	return (find(m_requirements.begin(), m_requirements.end(), req) != m_requirements.end());
}

void Data::display() {
	cout << "Domain name : " << m_domain->getName() << endl;
	cout << "Size : " << m_data.size() << endl;
	for(int i=0; i<m_data.size(); i++)
		cout << i << ": " << m_data[i] << endl;
	cout << "End of display." << endl;
}

