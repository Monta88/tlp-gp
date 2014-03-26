#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

#include <string> 
#include <vector>
#include "constant.h"
#include "type.h"
#include "fluent.h"
#include "action.h"

using namespace std;


class Domain {
	public:
		Domain(string name);
		Domain();
		virtual~Domain();
		string getName();
		string to_string();
		vector<string> listNameAction();
		void addType(Type * type);
		void addPredicate(Fluent * predicate);
		void addDurativeAction(Action * action);
		
	private:
		string m_name;
		vector<Constant*> m_constants;
		vector<Type*> m_types; 
		vector<Fluent*> m_predicates; 
		vector<Action*> m_actions; 
};

#endif // DOMAIN_H

