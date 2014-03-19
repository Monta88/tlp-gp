#pragma once


#ifndef TYPEDLIST_H
#define TYPEDLIST_H
 
#include <string>
#include <vector>
#include "type.h"

using namespace std;


class TypedList {
	public:
		TypedList();
		TypedList(vector<string> * types, vector<string> * list);
		~TypedList();
		vector<string> * getTypes();
		vector<string> * getList();
		string to_string();
	private:
		vector<string> m_types; // to store types which can be an (either type) : a list of one type is a simple type, and a list of more than one type is a (either type)
		vector<string> m_list; // to store variables/names
};

#endif // TYPEDLIST_H

