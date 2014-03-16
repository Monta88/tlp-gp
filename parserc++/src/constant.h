#pragma once


#ifndef CONSTANT_H
#define CONSTANT_H

#include <iostream> 
#include <string> 
#include "type.h"

using namespace std;


class Constant {
	public:
		Constant(string name, Type * type);
		Constant(string name);
		~Constant();
		string get_Name();
	private:
		string m_name;
		Type * m_type;
};

#endif // CONSTANT_H

