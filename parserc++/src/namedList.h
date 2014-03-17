#pragma once


#ifndef NAMEDLIST_H
#define NAMEDLIST_H
 
#include <string>
#include <vector>

using namespace std;


class NamedList {
	public:
		NamedList();
		NamedList(vector<string*> * name, vector<string*> * list);
		~NamedList();
		vector<string*> * get_Name();
		vector<string*> * get_List();
		string to_string();
	private:
		vector<string*> * m_list; // to store variables/names
		vector<string*> * m_name; // to store types/EITHER types
};

#endif // NAMEDLIST_H

