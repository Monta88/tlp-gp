#pragma once


#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Function {
	public:
		Function(string name, vector<Type*> return_type, vector< vector<Type*> > types_list);
		virtual ~Function();
		string getName();
		vector<Type*> * getReturnType();
		vector< vector<Type*> > * getTypesList();
		string to_string();
	private:
		string m_name;
		vector<Type*> m_return_type;
		vector< vector<Type*> > m_types_list;
};

#endif // FUNCTION_H

