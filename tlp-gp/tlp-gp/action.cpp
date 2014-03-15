#include "action.h"


Action::Action(string name):name(name)
{
}


Action::~Action()
{
}

string Action::get_Name()
{
	return name;
}

void Action::add_Parameters(Variable * parameter)
{
	parameters.push_back(parameter);
}

void Action::add_condition(Attributs att ,Fluent *fluent)
{	
	preconditions.push_back(make_pair(att,fluent));
}

void Action::add_effect(Attributs att ,Fluent *fluent)
{	
	effects.push_back(make_pair(att,fluent));
}









