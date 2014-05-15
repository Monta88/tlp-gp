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
	Graph2 m_graph2 = Graph2(m_domainptr, m_problemptr);
	m_graph2.generateGraph();
	m_vertex = m_graph2.getVertex();

	//DurativeAction *act = graph2.findAction(m_vertex,graph2.getInits(),m_problemptr->getGoals()->at(1).first);
	//cout << act->to_stringParam() <<endl;
	//cout << graph2.getInits()->to_string() <<endl;
	//cout << graph2.getGoals()->to_string() <<endl;

	//graph2.getVertex()->toVector();

	cout << m_domainptr->getPredicates()->size() << endl;
	vertexToActions(); //convert the graph to a vector of vectors
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

	//Buts ← Pre(A G ) ;
	//DONE

	//TODO
	//Pour chaque effet e de Eff(A I ) :
		//Ajouter un intervalle I d’apparition de la proposition e à Agenda(e) ;
	//Fin pour;*/


	int lvl = m_graph.size() - 1;
	cout << "ENTERING TLPGP1 CORE LOOP "<<lvl<<endl;
	// Tant que Buts ≠ ∅
	while(goals.size() != 0){
		cout << "IN THE LOOP"<<endl;

		//Pour chaque précondition p d’une action B, p ∈ Buts :
		for(auto it_precond = goals.begin(); it_precond != goals.end(); ++it_precond){
			cout << "plopp "<<endl;
			cout << (*it_precond)->to_string()<<endl;

			//Buts ← Buts – p ;

			//Sélectionner (* point de backtrack *) en utilisant l'heuristique, une action A
			//qui produit p pour B ;
			pair<DurativeAction* , int> p;
			p = selectAction(*it_precond,lvl,false);
			cout << p.first->to_string() <<" "<< p.second<<endl;

			//Buts ← Buts ∪ Pre(A) ;

			//Poser une contrainte de précédence entre A et B ;

			//Poser un intervalle I de maintien de précondition à Agenda(p) ;

			//Pour chaque intervalle I’ appartenant à Agenda(¬p) :
				//Poser une contrainte interdisant le recouvrement de I et I’.
			//Fin pour ;

			//Pour chaque effet e de A, (sauf pour p lorsque le label de p est un
			//singleton) :
				//Ajouter un intervalle I d’apparition de la proposition e à Agenda(e) ;

				//Pour chaque intervalle I’ de Agenda(¬e) :
					//Poser une contrainte interdisant le recouvrement de I et I’.
				//Fin pour ;
			//Fin pour ;


			//Vérifier la consistance de Contraintes (appel au solveur DTP) ;
			//En cas d’échec, retour au point de backtrack pour sélectionner une autre
			//action A ;
		}

		goals.clear();//remove
	}

	//Si Contraintes est consistant
	//Alors retourner le plan-solution flottant (actions sélectionnées et Contraintes)
	//Sinon il n'y a pas de solution à ce niveau du graphe ;
	//Fin si ;
	//Fin. TLPGP1
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

pair<DurativeAction* , int> Tlpgp1::selectAction(Fluent *effect, int level, bool sameLevel){
	if(sameLevel){
		for(auto it_act = m_graph.at(level).begin(); it_act != m_graph.at(level).end(); it_act++){
			if(m_graph2.compareFVF2((*it_act)->getEffectsF(),effect))
				return make_pair(*it_act,level);
		}
	}
	else{
		for(auto it_act = m_graph.at(level-1).begin(); it_act != m_graph.at(level-1).end(); it_act++){
			if(m_graph2.compareFVF2((*it_act)->getEffectsF(),effect))
				return make_pair(*it_act,level-1);
		}
	}
}
