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
#include "src/typedList.h"

using namespace std;

class Data {
public:
	Data();
	virtual ~Data();
	void add_Domain(string str);
	bool add_Constants(vector<TypedList*> * typed_list);
	bool add_Requirement(int req);
	bool is_Constant(Constant * constant);
	bool is_Requirement(int req);
	void display();
private:
	Domain * m_domain;
	vector<int> m_requirements;
	vector<Constant *> m_constants;
};

#endif /* DATA_H_ */
