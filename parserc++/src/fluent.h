#pragma once


#ifndef FLUENT_H
#define FLUENT_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Fluent {
	public:
		Fluent(string name);
		string get_Name();
		virtual ~Fluent();
	
	private:
		string m_name;
		vector<Type *> m_types;
};

#endif // FLUENT_H

