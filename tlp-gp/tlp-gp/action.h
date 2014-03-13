#pragma once


#ifndef ACTION_H
#define ACTION_H

#include <iostream> 
#include <string> 
#include <vector>
#include "variable.hpp"
#include "intervalle.hpp"
#include "fluent.hpp"
using namespace std;


class Action
{
public:
	Action(string name);
	string get_Name();
	void add_Parameters(Variable * parameter);
	void add_condition(Intervalle inter,Fluent *fluent);
	void add_effect(Intervalle inter,Fluent *fluent);
	virtual ~Action();
private:
	string name;
	vector<Variable *> parameters;
	vector< pair<Intervalle,Fluent *> > preconditions; 
	vector< pair<Intervalle,Fluent *> > effects; 
};
#endif // ACTION_H
