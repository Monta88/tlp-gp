#pragma once


#ifndef DURATIVEACTION_H
#define DURATIVEACTION_H

#include <string> 
#include <vector>
#include "variable.h"
#include "fluent.h"
#include "action.h"
#include "attribute.h"
#include "object.h"
using namespace std;


class DurativeAction : public Action {
	public:
		DurativeAction(string name);
		virtual ~DurativeAction();

		void addDuration(float duration);
		void addParameters(Variable parameter);
		float getDuration();
		void addCondition(Attribute att,Fluent *fluent);
		void addEffect(Attribute att,Fluent *fluent);
		void addNotCondition(Attribute att,Fluent *fluent);
		void addNotEffect(Attribute att,Fluent *fluent);

		bool isVariable(string name);
		Variable * getVariable(string name);
		
		vector< Fluent *> getPreconditions();
		bool isPredConditions(string * name,vector<vector<Type *>>);
		Fluent * getPredCondition(string * name,vector<vector<Type *>> );

		vector< Fluent *> getNotPreconditions();
		bool isPredNotConditions( string * name,vector<vector<Type *>>);
		Fluent * getPredNotCondition(string * name,vector<vector<Type *>> );
		vector< pair<Attribute, Fluent *> > getPreconditions2();
		vector< pair<Attribute, Fluent *> > getNotPreconditions2();
		vector< pair< Attribute,Fluent *> > getEffects();
		vector< pair< Attribute,Fluent *> > getNotEffects();
		
		vector< Fluent *> getEffectsF();			

		vector<Variable > * getParameters();
		string to_string();
		string to_stringParam();
		

		//constructeur par copie	
		
	private:
		vector<Variable> m_parameters;
		vector< pair< Attribute,Fluent *> > m_preconditions;
		vector< pair< Attribute,Fluent *> > m_not_preconditions; 
		vector< pair< Attribute,Fluent *> > m_effects; 
		vector< pair< Attribute,Fluent *> > m_not_effects; 
		float m_duration;
};

#endif // DURATIVEACTION_H

