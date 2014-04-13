/*
 * Graph.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: nathan
 */

#include "graph.h"


Graph::Graph(Domain *domain, Problem *problem) {
	// TODO Auto-generated constructor stub
	m_domainptr = domain;
	m_problemptr = problem;
}

Graph::Graph() {
	// TODO Auto-generated constructor stub
	m_domainptr = nullptr;
	m_problemptr = nullptr;
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

void Graph::generateGraph() {
	vector<pair<Fluent*, Attribute> > *inits, *goals ;
	vector<DurativeAction*> *actions ;
	vector< Fluent *> vect ;
	vector<pair<Attribute, Fluent*> > vect2 ;

	cout << m_domainptr->getName() <<endl;
	//data->display() ;

	inits = m_problemptr->getInits();
	goals = m_problemptr->getGoals();
	actions = m_domainptr->getActions();
	//actions = data->getDomain()->getActions();

	cout << "(generateGraph) Inits : " << inits->size() << endl;
	for(auto it = inits->begin(); it != inits->end(); ++it) //error with the first element
		cout << "\t" << (*it).first->to_string() << endl;

	cout << "(generateGraph) Goals : " << goals->size() << endl;
	for(auto it = goals->begin(); it != goals->end(); ++it) //error with the first element
		cout << "\t" << (*it).first->to_string() << endl;

	cout << "(generateGraph) Actions : " << actions->size()<< endl;
	for(auto it_act = actions->begin(); it_act != actions->end(); ++it_act) {//error with the first element
		cout << "\n" <<(*it_act)->getName() << endl;

		vect = (*it_act)->getPreconditions();
		cout << "(generateGraph) preconditions size:" << vect.size() << endl;
		for(auto it = vect.begin(); it != vect.end();++it){
			if(vect.size() != 0)
				cout << (*it)->to_string() <<endl;
			//cout << "plop" <<endl;
		}

		vect2 = (*it_act)->getEffects();
		cout << "(generateGraph) effects size:" << vect2.size() << endl;
		for(auto it = vect2.begin(); it != vect2.end(); ++it){
			if(vect2.size() != 0)
				cout << (*it).second->to_string() <<endl;
			//cout << "plop" <<endl;
		}

		vect2 = (*it_act)->getNotEffects();
		cout << "(generateGraph) not_effects size:" << vect2.size() << endl;
		for(auto it= vect2.begin(); it != vect2.end(); ++it){
			if(vect2.size() != 0)
				cout << it->second->to_string() <<endl;
			//cout << "plop" <<endl;
		}
	}

	for(auto it_act = actions->begin(); it_act != actions->end(); ++it_act) {//error with the first element
		actionUsable(*it_act, inits);
	}

	cout << "\n(generateGraph): END OF PRINT" <<endl;
}

bool Graph::actionUsable(DurativeAction* action, vector<pair<Fluent*, Attribute> > *fluents){
	bool res = true;
	vector< Fluent *> preconditions ;
	Fluent *fluent;

	preconditions = action->getPreconditions();

	cout << "\n" << "(actionUsable)"<< action->getName() << endl;
	cout << preconditions.size() <<" "<< fluents->size() <<endl;

	for(auto it_precond = preconditions.begin(); it_precond != preconditions.end(); ++it_precond){
		cout << (*it_precond)->to_string() <<endl;
		for(auto it_fluents = fluents->begin(); it_fluents != fluents->end(); ++it_fluents){
			fluent = (*it_fluents).first ;
			res = fluent->getPredicate()->getName()==(*it_precond)->getPredicate()->getName() ;
			//cout << (*it)->to_string() << " " << (*it_var).first->to_string() <<endl;
			//cout << "plop" <<endl;
			cout << fluent->getPredicate()->getName() << " " << res << endl;
			//cout << "\t" << (*it_var).first->to_string() <<endl;
		}
	}

	return res;
}
