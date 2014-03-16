#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream> 
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
		virtual~Domain();
		string getName();
		bool add_Constant(Constant * constant);
		bool add_Type(Type * type);
		bool add_Predicate(Fluent * predicate);
		bool add_Action(Action * action);
		bool is_Constant(Constant * constant);
		bool is_Type(Type * type);
		bool is_Predicate(Fluent * predicate);
		bool is_Action(Action * action);
		string to_string();
	private:
		string m_name;
		vector<Constant *> m_constants;
		vector<Type *> m_types; 
		vector<Fluent *> m_predicates; 
		vector<Action *> m_actions; 
};

#endif // DOMAIN_H

