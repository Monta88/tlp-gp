#include "variable.h"
Variable::Variable(string name,vector<Type *> type):m_name(name),m_type(type) {}

Variable::~Variable() {}

void Variable::addType(Type* type){
	m_type.push_back(type);
}

string Variable::getName(){
	return m_name;
}

vector<Type *>  Variable::getTypes(){
	return m_type;
}
