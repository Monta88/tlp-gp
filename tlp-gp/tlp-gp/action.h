#pragma once

#include <iostream> 
#include <string> 

using namespace std;

class action
{
public:
	action();
	~action();
private:
	string name;
	pair<string, string> parameters;
	bool isDurative;
	string preconditions; //TODO change type
	string effect; //TODO change type
};

