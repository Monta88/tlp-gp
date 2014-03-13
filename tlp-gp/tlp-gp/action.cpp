#include "action.hpp"


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

void Action::add_condition(Intervalle inter,Fluent *fluent)
{	
	preconditions.push_back(make_pair(inter,fluent));
}

void Action::add_effect(Intervalle inter,Fluent *fluent)
{	
	effects.push_back(make_pair(inter,fluent));
}









