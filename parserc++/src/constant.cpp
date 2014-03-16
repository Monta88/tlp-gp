#include "constant.h"

Constant::Constant(string name,Type * type):m_name(name), m_type(type) {}

Constant::Constant(string name):m_name(name),m_type(NULL) {}

Constant::~Constant() {}

string Constant::get_Name() {
	return m_name;
}

