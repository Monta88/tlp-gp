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
#include "src/typedList.h"


using namespace std;

class Data {
public:
	Data();
	virtual ~Data();
	void add_Domain(string str);
	bool add_Requirement(int req);
	bool is_Requirement(int req);
	bool add_Types(std::vector<TypedList*> * typedList_list);
	bool is_Type(string * type);
	Type * get_Type(vector<string*> * name_list);
	bool add_Constants(vector<TypedList*> * typedList_list);
	bool is_Constant(string * constant);
	bool add_Predicate(std::string * name, std::vector<TypedList*> * typedList_list);
	bool is_Predicate(string * predicate);
	void display();
private:
	Domain * m_domain;
	vector<int> * m_requirements;
	vector<string> *m_type_list;
	vector<string> * m_constant_list;
	vector<string> * m_predicate_list;
	vector<Type *> * m_types;
	vector<Constant *> * m_constants;
	vector<Fluent *> * m_predicates;
};

#endif /* DATA_H_ */
