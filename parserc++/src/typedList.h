#pragma once


#ifndef TYPEDLIST_H
#define TYPEDLIST_H
 
#include <string>
#include <vector>

using namespace std;


class TypedList {
	public:
		TypedList();
		TypedList(string * type, vector<string*> * list_name);
		~TypedList();
		string to_string();
	private:
		vector<string*> * m_list_name;
		string * m_type;
};

#endif // TYPEDLIST_H

