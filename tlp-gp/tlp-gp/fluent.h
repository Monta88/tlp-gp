#pragma once

#ifndef FLUENT_H
#define FLUENT_H

#include <string>
#include <vector>
#include "type.hpp"
using namespace std;

class Fluent
{
public:
	Fluent();
	virtual ~Fluent();
private:
	string name;
	vector<Type> types;
};
#endif // FLUENT_H
