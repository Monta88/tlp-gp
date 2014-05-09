#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

#include <string> 
#include <vector>
#include "type.h"
#include "constant.h"
#include "predicate.h"
#include "function.h"
#include "durative_action.h"
#include "lObjType.h"
#include "tools.h"

using namespace std;


class Domain {
	public:
		Domain(string name);
		Domain();
		virtual~Domain();
		string getName();
		string to_string();
		vector<string> listNameAction();
		void addTypes(vector<Type*> * types);
		void addConstants(vector<Constant*> * constants);
		void addPredicates(vector<Predicate*> * predicates);
		void addFunctions(vector<Function*> * functions);
		void addActions(vector<DurativeAction*> * actions);
		vector<DurativeAction *> * getActions();
		vector<lObjType> * getConstant();

		const vector<Predicate*>* getPredicates(){
			return m_predicates;
		}

	private:
		string m_name;
		vector<Type*> * m_types;
		vector<Constant*> * m_constants;
		vector<Predicate*> * m_predicates;
		vector<Function*> * m_functions;
		vector<DurativeAction *> * m_actions;
};

#endif // DOMAIN_H

