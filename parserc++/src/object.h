#pragma once


#ifndef OBJECT_H
#define OBJECT_H
 
#include <string> 
#include "type.h"

using namespace std;


class Object {
	public:
		Object(string name, vector<Type*> types);
		Object();
		~Object();
		string getName();
		vector<Type*> * getTypes();
		string to_string();
	private:
		string m_name;
		vector<Type*> m_types;
};

#endif // OBJECT_H

