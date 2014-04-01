#pragma once


#ifndef MENBER_H
#define MENBER_H

#include <string> 
#include "type.h"

using namespace std;


class virtual Member {
	public:
		Member(string name,vector<Type *> type);
		void addType(Type *type);
		Member getName();
		vector<Type *> getTypes();
		~Member();
	private:
		string m_name;
		vector<Type *> m_type;
};

#endif // MENBER_H

