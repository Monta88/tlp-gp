#pragma once


#ifndef ACTION_H
#define ACTION_H

#include <string> 
#include <vector>
#include "variable.h"
#include "fluent.h"
#include "attribute.h"

using namespace std;


class Action {
	public:
		Action(string name);
		virtual ~Action();
		string get_Name();
		void add_Parameters(Variable * parameter);
		void add_condition(Attribute att,Fluent *fluent);
		void add_effect(Attribute att,Fluent *fluent);
		string to_string();
	private:
		string m_name;
		vector<Variable *> m_parameters;
		vector< pair<Attribute,Fluent *> > m_preconditions; 
		vector< pair<Attribute,Fluent *> > m_effects; 
};

#endif // ACTION_H

