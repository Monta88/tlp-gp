#pragma once


#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>
#include "domain.h"
#include "variable.h"
#include "fluent.h"

using namespace std;


class Problem {
	public:
		Problem(string name,Domain * domain);
		Problem();
		~Problem();
		string getName();
		Domain * getDomain();

		const vector<pair<Fluent*, vector<Variable*> > >& getGoals() const {
			return m_goals;
		}

		const vector<pair<Fluent*, vector<Variable*> > >& getInits() const {
			return m_inits;
		}

		/*void add_Object(Variable *);
		void add_Init(Fluent *fluent,vector<Variable *> vars);
		void add_Goal(Fluent *fluent,vector<Variable *> vars);*/
	private:
		string m_name;
		Domain * m_domain;
		vector<Variable *> m_objects;
		vector< pair< Fluent *,vector<Variable *> > >  m_inits;
		vector< pair< Fluent *,vector<Variable *> > >  m_goals;
};

#endif // PROBLEM_H

