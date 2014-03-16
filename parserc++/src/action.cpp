#include "action.h"


Action::Action(string name):m_name(name) {}


Action::~Action() {}

string Action::get_Name() {
	return m_name;
}

void Action::add_Parameters(Variable * parameter) {
	m_parameters.push_back(parameter);
}

void Action::add_condition(Attribute att ,Fluent *fluent) {	
	m_preconditions.push_back(make_pair(att,fluent));
}

void Action::add_effect(Attribute att ,Fluent *fluent) {	
	m_effects.push_back(make_pair(att,fluent));
}

