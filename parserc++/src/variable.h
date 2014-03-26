#pragma once


#ifndef VARIABLE_H
#define VARIABLE_H

#include <string> 
#include "type.h"

using namespace std;


class Variable {
	public:
		Variable(string name,vector<Type *> type);
		void addType(Type *type);
		string getName();
		vector<Type *> getTypes();
		~Variable();
	private:
		string m_name;
		vector<Type *> m_type;
};

#endif // VARIABLE_H

