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
#include "src/function.h"
#include "src/object.h"
#include "src/problem.h"
#include "src/typedList.h"


using namespace std;

class Data {
public:
	Data();
	virtual ~Data();
	void addDomain(string * name);
	bool isDomain(string * name);
	bool addRequirement(int req);
	bool isRequirement(int req);
	bool addTypes(std::vector<TypedList*> * typedList_list);
	bool isType(string type);
	Type * getType(string name);
	bool addConstants(vector<TypedList*> * typedList_list);
	bool isConstant(string constant);
	bool addPredicate(string * name, vector<TypedList*> * typedList_list);
	bool isPredicate(string * name, vector<TypedList*> * typedList_list);
	bool addFunctions(vector< pair< string*, vector<TypedList*>* >* > * function_skeleton_list, vector<string> * return_type);
	bool addFunction(string * name, vector<Type*> return_type, vector<TypedList*> * typedList_list);
	bool isFunction(string * name, vector<TypedList*> * typedList_list);
	void addProblem(string * name);
	bool addObjects(vector<TypedList*> * typedList_list);
	bool isObject(string object);
	void display();
	void lexical_error(string msg);
	void fatal_error(string msg);
	
	Domain getDomain();

	//duratives-actions functions
	
	bool isAction(string name);
	Fluent * getFluent(string name,vector< vector<Type*> >);
private:
	Domain m_domain;
	vector<int> m_requirements;
	vector<string> m_type_list;
	vector<string> m_constant_list;
	vector<Type*> m_types;
	vector<Constant*> m_constants;
	vector<Fluent*> m_predicates;
<<<<<<< HEAD
	vector<Function*> m_functions;	
=======
	vector<Function*> m_functions;
	
	Problem m_problem;
	vector<string> m_object_list;
	vector<Object*> m_objects;
	
>>>>>>> c090d04dc6d3ff2354e7b5f854e364d3c8a93720
	vector<string> m_errors;
};

#endif /* DATA_H_ */
