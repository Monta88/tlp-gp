/*
 * Graph.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: nathan
 */

#include "graph.h"


Graph::Graph(Data *data) {
	// TODO Auto-generated constructor stub
	m_dataptr = data;
}

Graph::Graph() {
	// TODO Auto-generated constructor stub
	m_dataptr = nullptr;
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

void Graph::generateGraph() {
	vector<pair<Fluent*, Attribute> > inits ;
	vector<DurativeAction*> actions ;
	vector< Fluent *> vect ;
	vector<pair<Attribute, Fluent*> > vect2 ;

	cout << m_dataptr->getDomain()->getName() <<endl;
	//data->display() ;

	inits = m_dataptr->getInits() ;
	actions = m_dataptr->getActions();
	//actions = data->getDomain()->getActions();

	cout << "(generateGraph) Inits : " << inits.size() << endl;
	for(auto it = inits.begin()+1; it != inits.end(); ++it) //error with the first element
		cout << "\t" << (*it).first->to_string() << endl;

	cout << "(generateGraph) Actions : " << actions.size()<< endl;
	for(auto it_act = actions.begin()+1; it_act != actions.end(); ++it_act) {//error with the first element
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

	for(auto it_act = actions.begin()+1; it_act != actions.end(); ++it_act) {//error with the first element
		actionUsable(*it_act, inits);
	}

	cout << "\n(generateGraph): END OF PRINT" <<endl;
}

bool Graph::actionUsable(DurativeAction* action, vector<pair<Fluent*, Attribute> > var){
	bool res = true;
	vector< Fluent *> vect ;
	vector<pair<Fluent*, Attribute> > vect2 ;
	Fluent *fluent;

	vect = action->getPreconditions();
	vect2 = var;

	cout << "\n" << "(actionUsable)"<< action->getName() << endl;
	cout << vect.size() <<" "<< vect2.size() <<endl;

	for(auto it = vect.begin(); it != vect.end(); ++it){
		cout << (*it)->to_string() <<endl;
		for(auto it_var = vect2.begin(); it_var != vect2.end(); ++it_var){
			fluent = (*it_var).first ;
			//cout << (*it)->to_string() << " " << (*it_var).first->to_string() <<endl;
			cout << "plop" <<endl;
			cout << fluent->getPredicate()->getName() << endl;
			//cout << "\t" << (*it_var).first->to_string() <<endl;
		}
	}

	return res;
}
