#include "domain.h"


Domain::Domain(string name):name(name)
{
}


Domain::~Domain()
{
	types.~vector();
	predicates.~vector();
	actions.~vector();
}

void Domain::add_Type(Type * type)
{
	types.push_back(type);
}

void Domain::add_Predicate(Fluent * predicate)
{
	predicates.push_back(predicate);
}

void Domain::add_Action(Action * action)
{
	actions.push_back(action);
}

string Domain::to_string()
{
	string rep;
	rep = "Domain name" + name + " \n types : ";
	/*for (vector<Type *>::const_iterator it = types.begin(); it != types.end(); it++)
	{	
		rep +=(*it)->get_Name() + " , ";
	}
	/*rep += "\n Predicates : ";
	for (unsigned int i=0; i<predicates.size(); ++i)
	{	
		rep += predicates[i]->get_Name() + " , ";
	}
	/*rep += "\n Actions : ";
	for (unsigned int i=0; i<actions.size(); ++i)
	{	
		rep += actions[i].get_Name() + " , ";
	}*/
	return rep;
}







