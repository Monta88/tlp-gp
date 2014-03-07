#pragma once


#ifndef ACTION_H
#define ACTION_H

#include <iostream> 
#include <string> 
#include <vector>
#include "variable.hpp"
#include "intervalle.hpp"
#include "fluent.h"
using namespace std;


class Action
{
public:
	Action(string name);
	virtual ~Action();
private:
	string name;
	vector<Variable > parameters;
	vector< pair<Intervalle,Fluent> > preconditions; 
	vector< pair<Intervalle,Fluent> > effects; 
};
#endif // ACTION_H
