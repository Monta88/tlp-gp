#pragma once


#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream> 
#include <string> 
#include "type.h"

using namespace std;


class Variable {
	public:
		Variable(string name,Type type);
		~Variable();
	private:
		string m_name;
		Type m_type;
};

#endif // VARIABLE_H

