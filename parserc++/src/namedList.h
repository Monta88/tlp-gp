#pragma once


#ifndef NAMEDLIST_H
#define NAMEDLIST_H
 
#include <string>
#include <vector>

using namespace std;


class NamedList {
	public:
		NamedList();
		NamedList(string * name, vector<string*> * list);
		~NamedList();
		string * get_Name();
		vector<string*> * get_List();
		string to_string();
	private:
		vector<string*> * m_list;
		string * m_name;
};

#endif // NAMEDLIST_H

