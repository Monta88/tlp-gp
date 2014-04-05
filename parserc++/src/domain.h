#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

#include <string> 
#include <vector>
#include "constant.h"
#include "type.h"
#include "fluent.h"
#include "action.h"
#include "durative_action.h"
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
		void addPredicate(Predicate * predicate);
		void addDurativeAction(DurativeAction * action);
		vector<DurativeAction *> getActions();
		
		
	private:
		string m_name;
		vector<Constant*> m_constants;
		vector<Type*> m_types; 
		vector<Predicate*> m_predicates; 
		vector<DurativeAction *> m_actions; 
};

#endif // DOMAIN_H

