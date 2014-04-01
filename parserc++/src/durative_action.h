#pragma once


#ifndef DURATIVEACTION_H
#define DURATIVEACTION_H

#include <string> 
#include <vector>
#include "variable.h"
#include "fluent.h"
#include "action.h"
#include "attribut.h"
using namespace std;


class DurativeAction : public Action {
	public:
		DurativeAction(string name);
		virtual ~DurativeAction();

		void addDuration(float duration);
		void addParameters(Variable * parameter);

		void addCondition(Attribut att,Fluent *fluent);
		void addEffect(Attribut att,Fluent *fluent);
		void addNotCondition(Attribut att,Fluent *fluent);
		void addNotEffect(Attribut att,Fluent *fluent);

		bool isVariable(string name);
		Variable * getVariable(string name);
		
		vector< Fluent *> getPreconditions();
		bool isPredConditions(string * name,vector<vector<Type *>>);
		Fluent * getPredCondition(string * name,vector<vector<Type *>> );

		vector< Fluent *> getNotPreconditions();
		bool isPredNotConditions( string * name,vector<vector<Type *>>);
		Fluent * getPredNotCondition(string * name,vector<vector<Type *>> );
	private:
		vector<Variable *> m_parameters;
		vector< pair< Attribut,Fluent *> > m_preconditions;
		vector< pair< Attribut,Fluent *> > m_not_preconditions; 
		vector< pair< Attribut,Fluent *> > m_effects; 
		vector< pair< Attribut,Fluent *> > m_not_effects; 
		float m_duration;
};

#endif // DURATIVEACTION_H

