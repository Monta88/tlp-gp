#include "durative_action.h"


DurativeAction::DurativeAction(string name):Action(name) {}


DurativeAction::~DurativeAction() {}

void DurativeAction::addParameters(Variable * parameter) {
	m_parameters.push_back(parameter);
}

void DurativeAction::addCondition(Attribute att,Fluent *fluent) {	
	m_preconditions.push_back(make_pair(att,fluent));
}

void DurativeAction::addEffect(Attribute att,Fluent *fluent) {	
	m_effects.push_back(make_pair(att,fluent));
}

void DurativeAction::addDuration(float duration){
	m_duration=duration;
}

void DurativeAction::addNotCondition(Attribute att,Fluent *fluent){
	m_not_preconditions.push_back(make_pair(att,fluent));
}

void DurativeAction::addNotEffect(Attribute att,Fluent *fluent){
	m_not_effects.push_back(make_pair(att,fluent));
}

bool DurativeAction::isVariable(string name){
	for(vector<Variable *>::iterator it= m_parameters.begin();it != m_parameters.end();++it){
		if (((*it)->getName())==name) return true;
	}
	return false;
}

Variable * DurativeAction::getVariable(string name){
	for(vector<Variable *>::iterator it= m_parameters.begin();it != m_parameters.end();++it){
		if (((*it)->getName())==name) return (*it);
	}
	return new Variable("j'aime pas les warning",vector<Type *> ());
}

//preconditions
vector< Fluent *> DurativeAction::getPreconditions(){
	vector< Fluent *> prec =vector< Fluent *>();
	for (vector< pair< Attribute,Fluent *> >::iterator it = m_preconditions.begin(); it != m_preconditions.end(); ++it){
		prec.push_back((*it).second);
	}
	return prec;
}

bool DurativeAction::isPredConditions(string * name,vector<vector<Type *>> types){
	for (vector< pair< Attribute,Fluent *> >::iterator it_flu = m_preconditions.begin(); it_flu != m_preconditions.end(); ++it_flu) {
		if (((*name) == (*it_flu).second->getPredicate()->getName() ) && (equal(types.begin(), types.end(),(*it_flu).second->getPredicate()->getTypesList()->begin()) )) {
			return true;
		}
	}
	return false;
}

Fluent * DurativeAction::getPredCondition(string * name,vector<vector<Type *>> types){
	for (vector< pair< Attribute,Fluent *> >::iterator it_flu = m_preconditions.begin(); it_flu != m_preconditions.end(); ++it_flu) {
		if (((*name) == (*it_flu).second->getPredicate()->getName() ) && (equal(types.begin(), types.end(),(*it_flu).second->getPredicate()->getTypesList()->begin()) )) {
			return (*it_flu).second;
		}
	}
	return new Fluent(new Predicate("inexistant"));
}


//notprecondition
vector< Fluent *> DurativeAction::getNotPreconditions(){
	vector< Fluent *> prec =vector< Fluent *>();
	for (vector< pair< Attribute,Fluent *> >::iterator it = m_not_preconditions.begin(); it != m_not_preconditions.end(); ++it){
		prec.push_back((*it).second);
	}
	return prec;
}

bool DurativeAction::isPredNotConditions(string * name,vector<vector<Type *>> types){
	for (vector< pair< Attribute,Fluent *> >::iterator it_flu = m_not_preconditions.begin(); it_flu != m_not_preconditions.end(); ++it_flu) {
		if (((*name) == (*it_flu).second->getPredicate()->getName() ) && (equal(types.begin(), types.end(),(*it_flu).second->getPredicate()->getTypesList()->begin()) )) {
			return true;
		}
	}
	return false;
}

Fluent * DurativeAction::getPredNotCondition(string * name,vector<vector<Type *>> types){
	for (vector< pair< Attribute,Fluent *> >::iterator it_flu = m_not_preconditions.begin(); it_flu != m_not_preconditions.end(); ++it_flu) {
		if (((*name) == (*it_flu).second->getPredicate()->getName() ) && (equal(types.begin(), types.end(),(*it_flu).second->getPredicate()->getTypesList()->begin()) )) {
			return (*it_flu).second;
		}
	}
	return new Fluent(new Predicate("inexistant"));
}
