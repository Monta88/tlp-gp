#pragma once


#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>
#include "object.h"
#include "fluent.h"
#include "attribute.h"
#include "tools.h"
#include "lObjType.h"

using namespace std;


class Problem {
	public:
		Problem(string name);
		Problem();
		~Problem();
		string getName();
		
		vector<pair<Fluent*, Attribute> > * getGoals(){
			return m_goals;
		}
				
		vector<pair<Fluent*, Attribute> > * getInits(){
			return m_inits;
		}
	
		vector<lObjType> * getObjects();

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

