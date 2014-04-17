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
	DurativeAction goalsAction("Goals") ;

	vector< Fluent *> vect ;
	vector<pair<Attribute, Fluent*> > vect2,vect3,inits2 ;
	Fluent *f;
	Attribute a;
	pair<Attribute, Fluent*> p;

	cout << m_domainptr->getName() <<endl;
	//data->display() ;

	inits = m_problemptr->getInits();
	goals = m_problemptr->getGoals();
	actions = m_domainptr->getActions();
	//actions = data->getDomain()->getActions();

	for(auto it = goals->begin(); it != goals->end(); ++it){
		f = (*it).first;
		a = (*it).second;
		goalsAction.addCondition(a,f);
	}
	cout << "GOALS: " << goalsAction.getPreconditions().size() << endl;


	// inits2 = inits inverted
	for(auto it = inits->begin(); it != inits->end(); ++it){
		f = (*it).first;
		a = (*it).second;
		p.first = a;
		p.second = f;
		inits2.push_back(p);
	}

	/*cout << "(generateGraph) Inits : " << inits->size() << endl;
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
	}*/

	vect2.clear();
	vect2.insert(vect2.end(),inits2.begin(),inits2.end());
	cout << "\nETAPE 1: " << vect2.size() << endl;
	for(auto it_act = actions->begin(); it_act != actions->end(); ++it_act) {
		if(actionUsable(*it_act, &inits2)){
			cout << (*it_act)->getName() << endl;

			vect3 = (*it_act)->getEffects();
			vect2.insert(vect2.end(), vect3.begin(), vect3.end());
		}
	}
	cout << "RESULT: " << vect2.size() << " " << actionUsable(&goalsAction, &inits2)<< endl;

	cout << "\nETAPE 2: " << vect2.size() << endl;
	for(auto it_act = actions->begin(); it_act != actions->end(); ++it_act) {
		if(actionUsable(*it_act, &vect2)){
			cout << (*it_act)->getName() << endl;

			vect3 = (*it_act)->getEffects();
			vect2.insert(vect2.end(), vect3.begin(), vect3.end());
		}
	}
	cout << "RESULT: " << vect2.size() << " " << actionUsable(&goalsAction, &inits2)<< endl;

	cout << "\nETAPE 3: " << vect2.size() << endl;
	for(auto it_act = actions->begin(); it_act != actions->end(); ++it_act) {
		if(actionUsable(*it_act, &vect2)){
			cout << (*it_act)->getName() << endl;

			vect3 = (*it_act)->getEffects();
			vect2.insert(vect2.end(), vect3.begin(), vect3.end());
		}
	}
	cout << "RESULT: " << vect2.size() << " " << actionUsable(&goalsAction, &inits2)<< endl;

	cout << "\n(generateGraph): END OF PRINT" <<endl;
}

// Given a list of fluents, return if the action is activable (ie all preconds are satisfied)
bool Graph::actionUsable(DurativeAction* action, vector<pair<Attribute, Fluent*> > *fluents){
	bool res=true ,res2 = true;
	vector< Fluent *> preconditions ;
	Fluent *fluent;

	preconditions = action->getPreconditions();

	//cout << "\n" << "(actionUsable)"<< action->getName();
	//cout << preconditions.size() <<" "<< fluents->size() <<endl;

	for(auto it_precond = preconditions.begin(); it_precond != preconditions.end(); ++it_precond){
		//cout << "preconditons: " << (*it_precond)->to_string() <<endl;
		res2 = false;
		for(auto it_fluents = fluents->begin(); it_fluents != fluents->end(); ++it_fluents){
			fluent = (*it_fluents).second ;
			res2 = res2 || fluent->getPredicate()->getName()==(*it_precond)->getPredicate()->getName() ;
			//cout << res2 <<endl;
			//cout << fluent->getPredicate()->getName() << " " << res2 << endl;
			//cout << "\t" << (*it_var).first->to_string() <<endl;
		}
		//cout << "res: " << res << " res2: " <<res2 <<endl;
		res = (res && res2) ;;
	}

	//cout << " -> " << res ;;

	return res;
}
