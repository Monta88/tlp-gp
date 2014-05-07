/*!
 * \file tlpgp1.cpp
 * \brief [wip]Contains the core code of tlpgp1
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date Apr 23, 2014
 */

#include "tlpgp1.h"

Tlpgp1::Tlpgp1() {
	m_domainptr = nullptr;
	m_problemptr = nullptr;
}

Tlpgp1::Tlpgp1(Domain *domain, Problem *problem) :
		m_domainptr(domain), m_problemptr(problem) {
}

Tlpgp1::~Tlpgp1() {
	// TODO Auto-generated destructor stub
}

void Tlpgp1::constructGraph() {
	Graph2 graph2 = Graph2(m_domainptr, m_problemptr);
	graph2.generateGraph();
	m_vertex = graph2.getVertex();

	//DurativeAction *act = graph2.findAction(m_vertex,graph2.getInits(),m_problemptr->getGoals()->at(1).first);
	//cout << act->to_stringParam() <<endl;
	//cout << graph2.getInits()->to_string() <<endl;
	//cout << graph2.getGoals()->to_string() <<endl;

	//graph2.getVertex()->toVector();

	cout << m_domainptr->getPredicates()->size() << endl;
	vertexToActions();
	cout << "gggggggggggggggggg" << m_graph.size() << endl;
	//cout<<	m_graph[0][0]->to_stringParam() <<endl;
	//cout << m_graph.at(0)->to_stringParam() <<endl;
	//cout << m_graph.at(0)[0]->to_string() <<endl;

	for (int i = 0; i < m_graph.size(); ++i) {
		cout << "ccccc " << m_graph[i].size() << endl;
		for (auto it = m_graph[i].begin(); it != m_graph[i].end(); ++it) {
			cout << "actionaction" << (*it)->getName() << endl;
		}
	}
	//m_vertex->to_string();

	vector<Fluent *> goals = vector<Fluent *>();
	vector<Fluent *> inits = vector<Fluent *>();

	//for(auto it = graph2.getGoals()->getPreconditions().begin(); it != graph2.getGoals()->getPreconditions().end(); ++it)
	//cout << (*it)->to_string() <<endl;
	//goals.push_back(*it);

	//for(auto it = graph2.getGoals()->getNotPreconditions().begin(); it != graph2.getGoals()->getNotPreconditions().end(); ++it)
	//cout << (*it)->to_string() <<endl;
	//goals.push_back(*it);
	cout << "azzae" << endl;
	;
	cout << m_problemptr->getGoals()->size() << endl;
	;
	for (auto it = m_problemptr->getGoals()->begin();
			it != m_problemptr->getGoals()->end(); ++it) {
		cout << (*it).first->to_string() << "  aaa  "
				<< (*it).second.to_string() << endl;
		goals.push_back((*it).first);
	}
	cout << "vvsdsd" << goals.size() << endl;
	;

	for (auto it = m_problemptr->getInits()->begin();
			it != m_problemptr->getInits()->end(); ++it) {
		cout << (*it).first->to_string() << "  bbb  "
				<< (*it).second.to_string() << endl;
		inits.push_back((*it).first);
	}
	cout << "vdgsdsg" << inits.size() << endl;
	;
}

void Tlpgp1::vertexToActions() {
	cout << "aaaaaaaaaaaaaa" << m_vertex->getActions()->size() << endl;
	vector<DurativeAction *> * act = m_vertex->getFather()->getActions();
	cout << "aaaaaaaaaaaaaa" << act->size() << endl;

	//m_graph = vector<vector<DurativeAction* >> (10);
	m_graph.clear();

	Vertex * vertex = new Vertex(m_vertex);
	int state = 0;
	do {
		vertex = vertex->getFather();
		//cout<<"\zzzzzzzzzzzzzzzz "<<state<<"nbaction : "<<vertex->getActions()->size()<<"\n";
		/*for(vector<DurativeAction *>::iterator it = vertex->getActions()->begin() ; it != vertex->getActions()->end() ; ++it){
		 //cout<<"qqqqqqqqqqqqqq "<<(*it)->to_string()<<"\n";
		 m_graph[state].push_back(*it);
		 }*/
		m_graph.push_back(*(vertex->getActions()));
		state++;

	} while (vertex->getFather() != NULL);

	reverse(m_graph.begin(), m_graph.end());
	//m_graph.pop_back();

}

// selectAction(vector DurativeAction*, effect)
