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

	cout << "(test) Inits : " << inits.size() << endl;
	for(auto it = inits.begin()+1; it != inits.end(); ++it) //error with the first element
		cout << "\t" << (*it).first->to_string() << endl;

	cout << "(test) Actions : " << actions.size()<< endl;
	for(auto it_act = actions.begin()+1; it_act != actions.end(); ++it_act) {//error with the first element
		cout << "\n" <<(*it_act)->getName() << endl;

		vect = (*it_act)->getPreconditions();
		cout << "preconditions size:" << vect.size() << endl;
		for(auto it = vect.begin(); it != vect.end();++it){
			if(vect.size() != 0)
				cout << (*it)->to_string() <<endl;
			//cout << "plop" <<endl;
		}

		vect2 = (*it_act)->getEffects();
		cout << "effects size:" << vect2.size() << endl;
		for(auto it = vect2.begin(); it != vect2.end(); ++it){
			if(vect2.size() != 0)
				cout << (*it).second->to_string() <<endl;
			//cout << "plop" <<endl;
		}

		vect2 = (*it_act)->getNotEffects();
		cout << "not_effects size:" << vect2.size() << endl;
		for(auto it= vect2.begin(); it != vect2.end(); ++it){
			if(vect2.size() != 0)
				cout << it->second->to_string() <<endl;
			//cout << "plop" <<endl;
		}
	}

	cout << "\nGRAPH: END OF PRINT" <<endl;
}

