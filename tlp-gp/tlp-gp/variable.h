
#ifndef VARIABLE_H
#define VARIABLE_H
#include <iostream> 
#include <string> 
#include "type.h"
using namespace std;

class Variable
{
public:
	Variable(string name,Type type);
	~Variable();
private:
	string name;
	Type type;
};

#endif // VARIABLE_H
