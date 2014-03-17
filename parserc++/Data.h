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
#include "src/domain.h"
#include "src/type.h"
#include "src/constant.h"
#include "src/fluent.h"
#include "src/namedList.h"


using namespace std;

class Data {
public:
	Data();
	virtual ~Data();
	void add_Domain(string str);
	bool add_Requirement(int req);
	bool is_Requirement(int req);
	bool add_Constants(vector<NamedList*> * named_list);
	bool is_Constant(Constant * constant);
	bool add_Predicate(std::string * name, std::vector<NamedList*> * named_list);
	bool is_Predicate(Fluent * predicate);
	void display();
private:
	Domain * m_domain;
	vector<int> m_requirements;
	vector<Type *> m_types; // for your Martin ;)
	vector<Constant *> m_constants;
	vector<Fluent *> m_predicates;
};

#endif /* DATA_H_ */
