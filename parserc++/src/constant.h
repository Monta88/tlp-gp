#pragma once


#ifndef CONSTANT_H
#define CONSTANT_H
 
#include <string> 
#include "type.h"

using namespace std;


class Constant {
	public:
		Constant(string * name, Type * type);
		Constant();
		~Constant();
		string * get_Name();
		Type * get_Type();
		string to_string();
	private:
		string * m_name;
		Type * m_type;
};

#endif // CONSTANT_H

