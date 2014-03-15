#pragma once


#ifndef ACTION_H
#define ACTION_H

#include <iostream> 
#include <string> 
#include <vector>
#include "variable.h"
#include "fluent.h"
#include "attribut.h"
using namespace std;


class Action
{
public:
	Action(string name);
	string get_Name();
	void add_Parameters(Variable * parameter);
	void add_condition(Attributs att,Fluent *fluent);
	void add_effect(Attributs att,Fluent *fluent);
	virtual ~Action();
private:
	string name;
	vector<Variable *> parameters;
	vector< pair<Attributs,Fluent *> > preconditions; 
	vector< pair<Attributs,Fluent *> > effects; 
};
#endif // ACTION_H
