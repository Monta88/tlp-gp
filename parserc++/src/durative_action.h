#pragma once


#ifndef DURATIVEACTION_H
#define DURATIVEACTION_H

#include <string> 
#include <vector>
#include "variable.h"
#include "fluent.h"
#include "attribute.h"
#include "action.h"
using namespace std;


class DurativeAction:Action {
	public:
		DurativeAction(string name);
		virtual ~DurativeAction();
		void addParameters(Variable * parameter);
		void addCondition(Attribute att,Fluent *fluent);
		void addEffect(Attribute att,Fluent *fluent);
		void addDuration(float duration);
		bool isVariable(string name);
		Variable * getVariable(string name);
	private:
		vector<Variable *> m_parameters;
		vector< pair<Attribute,Fluent *> > m_preconditions; 
		vector< pair<Attribute,Fluent *> > m_effects; 
		float m_duration;
};

#endif // DURATIVEACTION_H

