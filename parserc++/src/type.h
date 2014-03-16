#pragma once


#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <string>

using namespace std;


class Type {
	public:
		Type(string name);        
		string getType();
		virtual ~Type();
		string get_Name();
	protected:
		string m_name;
	private:
};

#endif // TYPE_H

