#include "type.h"

Type::Type(string name):m_name(name) {}

Type:: ~Type() {} 
     
string Type::get_Name() {
	return m_name;
}

string Type::to_string() {
	return m_name;
}

