/*
 * Data.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#include "Data.h"

Data::Data() {
	// TODO Auto-generated constructor stub
	this->m_data = std::vector<string> ();
}

Data::~Data() {
	// TODO Auto-generated destructor stub
}

void Data::Add(string str){
	this->m_data.push_back(str);
}

void Data::Display() {
	std::cout << "size: " << this->m_data.size() << std::endl;
	for(int i=0; i<m_data.size(); i++)
		std::cout << i << ": " << m_data[i] << std::endl;
	std::cout << "end of display" << std::endl;
}

