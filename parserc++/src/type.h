#pragma once


#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <vector>

using namespace std;


class Type {
	public:
		Type(string name, vector<Type*> parents);
		Type(string name);
		Type();
		virtual ~Type();
		void addParents(vector<Type*> parents);
		string getName();
		vector<Type*> * getParents();
		bool isOneOfParents(string name);
		string to_string();
	private:
		string m_name;
		vector<Type*> m_parents;
};

#endif // TYPE_H

