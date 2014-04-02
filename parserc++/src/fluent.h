#pragma once


#ifndef FLUENT_H
#define FLUENT_H

#include <string>
#include <vector>
#include "member.h"
#include "predicate.h"

using namespace std;


class Fluent {
	public:
		Fluent(Predicate * predicate, vector<Member*> members_list);
		Fluent(Predicate * predicate);
		virtual ~Fluent();
		Predicate * getPredicate();
		vector<Member*> * getMembersList();
		void addMember(vector<Member*> member);
		void addMember(Member * member);
		string to_string();
	private:
		Predicate * m_predicate;
		vector<Member*> m_members_list;
};

#endif // FLUENT_H

