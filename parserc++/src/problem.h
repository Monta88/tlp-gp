#pragma once


#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>
#include "object.h"
#include "fluent.h"
#include "attribute.h"

using namespace std;


class Problem {
	public:
		Problem(string name);
		Problem();
		~Problem();
		string getName();
		
		const vector<pair<Fluent*, Attribute> > * getGoals() const {
			return m_goals;
		}

		const vector<pair<Fluent*, Attribute> > * getInits() const {
			return m_inits;
		}

		void addObjects(vector<Object*> * objects);
		void addInits(vector<pair<Fluent*, Attribute> > * inits);
		void addGoals(vector<pair<Fluent*, Attribute> > * goals);
	private:
		string m_name;
		vector<Object*> * m_objects;
		vector<pair<Fluent*, Attribute> > * m_inits;
		vector<pair<Fluent*, Attribute> > * m_goals;
};

#endif // PROBLEM_H

