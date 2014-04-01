#pragma once


#ifndef MENBER_H
#define MENBER_H

#include <string> 
#include "type.h"

using namespace std;


class Member {
	public:
		Member(string name,vector<Type *> type);
		virtual void addType(Type *type);
		virtual string getName();
		virtual vector<Type *> getTypes();
		virtual ~Member();
	private:
		string m_name;
		vector<Type *> m_type;
};

#endif // MENBER_H

