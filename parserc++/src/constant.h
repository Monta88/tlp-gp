#pragma once


#ifndef CONSTANT_H
#define CONSTANT_H
 
#include <string> 
#include "type.h"

using namespace std;


class Constant {
	public:
		Constant(string name, vector<Type*> types);
		Constant();
		~Constant();
		string getName();
		vector<Type*> * getTypes();
		string to_string();
	private:
		string m_name;
		vector<Type*> m_types;
};

#endif // CONSTANT_H

