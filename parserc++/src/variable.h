#pragma once


#ifndef VARIABLE_H
#define VARIABLE_H

#include "member.h"


class Variable : public Member {
	public:
		Variable(string name, vector<Type*> types);
		Variable();
		string to_string();
		string getClass();
};

#endif // VARIABLE_H

