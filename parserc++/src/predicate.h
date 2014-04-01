#pragma once


#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Predicate {
	public:
		Predicate(string name);
		Predicate(string name, vector< vector<Type*> > types_list);
		virtual ~Predicate();
		string getName();
		vector< vector<Type*> > * getTypesList();
		string to_string();
	private:
		string m_name;
		vector< vector<Type*> > m_types_list;
};

#endif // PREDICATE_H

