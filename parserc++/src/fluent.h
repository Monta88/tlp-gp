#pragma once


#ifndef FLUENT_H
#define FLUENT_H

#include <string>
#include <vector>
#include "type.h"
#include "namedList.h"

using namespace std;


class Fluent {
	public:
		Fluent(string * name, std::vector<NamedList*> * named_list);
		virtual ~Fluent();
		string get_Name();
		vector<Type *> get_Types();
		string to_string();
	private:
		string m_name;
		vector<Type *> m_types;
};

#endif // FLUENT_H

