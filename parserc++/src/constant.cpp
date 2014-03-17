#include "constant.h"

Constant::Constant(string name,Type * type):m_name(name), m_type(type) {}

Constant::~Constant() {}

string Constant::get_Name() {
	return m_name;
}

string Constant::to_string() {
	return "Constant : " + m_name + " - " + m_type->get_Name(); // TODO modify m_type->get_Name() in m_type->to_string()
}

