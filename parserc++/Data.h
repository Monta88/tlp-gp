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
	void add(string str);
	void add_Domain(string str);
	bool add_Constant(string name);
	bool add_Constant(string name, string type);
	bool add_Requirement(int req);
	bool is_Constant(string name);
	bool is_Constant(string name, string type);
	bool is_Requirement(int req);
	void display();
private:
	vector<string> m_data ;
	Domain * m_domain;
	vector<int> m_requirements;
	vector<Constant *> m_constants;
};

#endif /* DATA_H_ */
