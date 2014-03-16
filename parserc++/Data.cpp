/*
 * Data.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#include "Data.h"

Data::Data() {
	// TODO Auto-generated constructor stub
	this->data = std::vector<string> ();
}

Data::~Data() {
	// TODO Auto-generated destructor stub
}

void Data::Add(string str){
	this->data.push_back(str);
}

void Data::Display() {
	std::cout << "size: " << this->data.size() << std::endl;
	for(int i=0; i<data.size(); i++)
		std::cout << i << ": " << data[i] << std::endl;
	std::cout << "end of display" << std::endl;
}

