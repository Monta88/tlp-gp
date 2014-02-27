#pragma once

#include <list> 
#include <string> 


using namespace std;

class domain
{
public:
	domain();
	~domain();
private:
	string name;
	list<string> requirements;
	list<pair<list<string>, string>> types; // list of pair of ([obj1,obj2...],type)
	string predicates; //TODO list of predicates
	string actions; //TODO structure(map?) of actions
};

