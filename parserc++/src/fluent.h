#pragma once


#ifndef FLUENT_H
#define FLUENT_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Fluent {
	public:
		Fluent(string name, vector< vector<Type*> > types_list);
		virtual ~Fluent();
		string getName();
		vector< vector<Type*> > * getTypesList();
		string to_string();
	private:
		string m_name;
		vector< vector<Type*> > m_types_list;
};

#endif // FLUENT_H

