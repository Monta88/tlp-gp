#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream> 
#include <string> 
#include <vector>
#include "type.h"
#include "fluent.h"
#include "action.h"
using namespace std;

class Domain
{
public:
	Domain();
	virtual~Domain();
private:
	string name;
	vector<Type> types; 
	vector<Fluent> predicates; 
	vector<Action> actions; 
};
#endif // DOMAIN_H
