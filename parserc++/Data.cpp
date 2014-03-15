/*
 * Data.cpp
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#include "Data.h"

namespace std {

Data::Data() {
	// TODO Auto-generated constructor stub
	this->data = vector<string> (50);
}

Data::~Data() {
	// TODO Auto-generated destructor stub
}

void Data::Add(string str){
	this->data.push_back(str);
}

} /* namespace std */
