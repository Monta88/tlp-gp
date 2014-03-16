/*
 * Data.h
 *
 *  Created on: Mar 15, 2014
 *      Author: nathan
 */

#ifndef DATA_H_
#define DATA_H_

#include <string>
#include <vector>
#include <iostream>
#include "src/domain.h"

using namespace std;

class Data {
public:
	Data();
	virtual ~Data();
	void Add(std::string str);
	void Add_Domain(std::string str);
	void Add_Requirement(int req);
	bool Is_Requirement(int req);
	void Display();
private:
	std::vector<string> m_data ;
	Domain * m_domain;
	vector<int> m_requirements;
};

#endif /* DATA_H_ */
