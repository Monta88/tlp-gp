#pragma once


#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <vector>

using namespace std;


class Type {
	public:
		Type(Type * parent, vector<string*> * name_list);
		Type(vector<string*> * parent, string * name);
		Type(vector<string*> * name_list);
		Type();
		virtual ~Type();
		vector<string*> * get_Name_List();
		Type * get_Parent();
		string to_string();
	private:
		Type * m_parent;
		vector<string*> * m_name_list; // a type can be an (either type) : a list of one type is a simple type, and a list of more than one type is a (either type)
};

#endif // TYPE_H

