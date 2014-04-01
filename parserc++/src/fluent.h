#pragma once


#ifndef FLUENT_H
#define FLUENT_H

#include <string>
#include <vector>
#include "variable.h"
#include "predicate.h"

using namespace std;


class Fluent {
	public:
		Fluent(Predicate * predicate);
		virtual ~Fluent();
		Predicate * getPredicate();
		void addVariable(vector< Variable *> variable);
		void addVariable(Variable * variable);
		string to_string();
	private:
		Predicate * m_predicate;
		vector< Variable *> m_variables;
};

#endif // FLUENT_H

