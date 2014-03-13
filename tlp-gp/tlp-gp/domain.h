#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream> 
#include <string> 
#include <vector>
#include "type.hpp"
#include "fluent.hpp"
#include "action.hpp"
using namespace std;

class Domain
{
public:
	Domain(string name);
	virtual~Domain();
	void add_Type(Type * type);
	void add_Predicate(Fluent * predicate);
	void add_Action(Action * action);
	string to_string();
private:
	string name;
	vector<Type *> types; 
	vector<Fluent *> predicates; 
	vector<Action *> actions; 
};
#endif // DOMAIN_H
