#include "fluent.h"


Fluent::Fluent(string name):m_name(name) {}

Fluent::~Fluent() {}

string Fluent::get_Name() {
	return m_name;
}

