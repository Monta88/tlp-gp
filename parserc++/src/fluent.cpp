#include "fluent.h"


Fluent::Fluent(Predicate * predicate):m_predicate(predicate) {}

Fluent::~Fluent() {}

Predicate *  Fluent::getPredicate() {
	return m_predicate;
}

string Fluent::to_string() {
	return " changement de structure ; à implémenté";
}


void Fluent::addVariable(vector< Variable *> variable){
	 m_variables=variable;
}
		

void Fluent::addVariable(Variable * variable){
	m_variables.push_back(variable);
}
