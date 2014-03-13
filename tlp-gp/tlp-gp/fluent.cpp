#include "fluent.hpp"


Fluent::Fluent(string name):name(name)
{
}


Fluent::~Fluent()
{
}

string Fluent::get_Name()
{
	return name;
}
