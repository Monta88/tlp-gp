#pragma once


#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Member {
	public:
		Member(string name, vector<Type*> types);
		Member();
		virtual ~Member();
		string getName();
		vector<Type*> * getTypes();
		virtual string to_string();
	protected:
		string m_name;
		vector<Type*> m_types;
};

#endif // MEMBER_H
