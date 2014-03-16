/*
 * Data.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#include "Data.h"

#include <algorithm>

Data::Data() {
	// TODO Auto-generated constructor stub
	this->m_data = vector<string> ();
	this->m_requirements = vector<int> ();
}

Data::~Data() {
	// TODO Auto-generated destructor stub
}

void Data::Add(string str) {
	this->m_data.push_back(str);
}

void Data::Add_Domain(string str) {
	this->m_domain = new Domain(str);
}

void Data::Add_Requirement(int req) {
	this->m_requirements.push_back(req);
}

bool Data::Is_Requirement(int req) {
	return (find(m_requirements.begin(), m_requirements.end(), req) != m_requirements.end());
}

void Data::Display() {
	cout << "Domain name : " << this->m_domain->getName() << endl;
	cout << "Size : " << this->m_data.size() << endl;
	for(int i=0; i<m_data.size(); i++)
		cout << i << ": " << m_data[i] << endl;
	cout << "End of display." << endl;
}

