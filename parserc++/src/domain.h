#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream> 
#include <string> 
#include <vector>
#include "type.h"
#include "fluent.h"
#include "action.h"

using namespace std;


class Domain {
	public:
		Domain(string name);
		virtual~Domain();
		void add_Type(Type * type);
		void add_Predicate(Fluent * predicate);
		void add_Action(Action * action);
		string to_string();
	private:
		string m_name;
		vector<Type *> m_types; 
		vector<Fluent *> m_predicates; 
		vector<Action *> m_actions; 
};

#endif // DOMAIN_H

