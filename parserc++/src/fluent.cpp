#include "fluent.h"


Fluent::Fluent(Predicate * predicate, vector<Member*> members_list):m_predicate(predicate), m_members_list(members_list) {}

Fluent::Fluent(Predicate * predicate):m_predicate(predicate) {}

Fluent::~Fluent() {}

Predicate * Fluent::getPredicate() {
	return m_predicate;
}

vector<Member*> * Fluent::getMembersList() {
	return &m_members_list;
}

void Fluent::addMember(vector<Member*> member){
	 m_members_list = member;
}
		

void Fluent::addMember(Member * member){
	m_members_list.push_back(member);
}

string Fluent::to_string() {
	string str = "Fluent " + m_predicate->getName() + " :";
	for(vector<Member*>::iterator it = m_members_list.begin(); it != m_members_list.end(); ++it) {
		str += " " + (*it)->getName();
	}
	return str;
}

