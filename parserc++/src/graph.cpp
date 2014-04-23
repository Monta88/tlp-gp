/*
 * Graph.cpp
 *
 *  Created on: Apr 9, 2014
 */

#include "graph.h"


Graph::Graph(Domain *domain, Problem *problem):m_domainptr(domain), m_problemptr(problem){}

Graph::Graph() {
	// TODO Auto-generated constructor stub
	m_domainptr = nullptr;
	m_problemptr = nullptr;
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

//return the list of object sorted to instanciated the actions
vector<DurativeAction * > * Graph::instanceActions(){
	vector<DurativeAction *> * m_actions = m_domainptr->getActions();
	vector<lObjType> * objects = m_problemptr->getObjects();
	vector<DurativeAction * >* ret = new vector<DurativeAction *>();	
	vector<DurativeAction *>* temp ;
	vector<vector<Object * > >*objects_need = new vector<vector<Object * > >();
	vector<Object * > temp2;
	vector<Type * > temp3;
	Tools tool =Tools();
	bool find;
	for (vector<DurativeAction *>::iterator it_act = m_actions->begin(); it_act != m_actions-> end() ;++it_act){
		//build list object the action need
		for(vector<Variable >::iterator it_param = (*it_act)->getParameters()->begin() ; it_param != (*it_act)->getParameters()->end() ; ++it_param){	
			find = false;
			for (vector<lObjType>::iterator it=objects->begin(); it != objects->end(); ++it){
				temp3 =((*it).getType());	
				if (tool.compareVectorType(&temp3,(*it_param).getTypes())) {
					temp2=(*it).getObject();
					find = true;
				}
			}
			if ( ! find){
				temp2 = vector<Object * >();
			}
			if ( temp2.size() != 0){
				objects_need->push_back(temp2);
			}
		}
		
		temp = instanciation(objects_need,(*it_act));
		for (vector<DurativeAction *>::iterator it_ins = temp->begin(); it_ins != temp->end() ; ++it_ins){
			ret->push_back((*it_ins));
		}
		objects_need->clear();
		temp->clear();
		temp2.clear();
		temp3.clear();
	}
	return ret;
}

// all the objects must be usefull  , return all the action instanciated with all the objects combinaisons 
vector<DurativeAction *>* Graph::instanciation(vector<vector<Object * > > * objects,DurativeAction * action){
	vector<vector<Object  > >* all_instance = new vector<vector<Object >>();
	vector<Object > instance =  vector<Object  >();
	int * tabMax = new int[objects->size()];
	int * tab = new int[objects->size()];
	string temp;
	vector<Member*> members_list;
	Fluent f;
	for (unsigned int i = 0 ;i < objects->size();++i){
		tab[i] =0;
		tabMax[i]=objects->at(i).size();
	}
	int i;
	bool increment,continu=true;
	// step 1 : create all combinason of object for the action
	while(continu){
		i =0;
		instance =  vector<Object  >();
		for (vector<vector<Object * >  >::iterator it = objects->begin() ; it != objects->end() ; ++it ){
			instance.push_back(*((*it).at(tab[i])));	
			i++;
			
		}
		all_instance->push_back(instance);
		i=0;
		increment=true;
		while(increment){
			tab[i]++;
			if(tab[objects->size()-1]==tabMax[objects->size()-1]){
				increment=false;
				continu = false;
			} else {
				if (tab[i]==tabMax[i]){
					tab[i]=0;
					i++;
				} else {
					increment =false;
				}
			}
		}
	}
	vector<DurativeAction *>* ret=new vector<DurativeAction *>();
	DurativeAction *actualAction = new DurativeAction(*action);
	for(unsigned i=0;i<all_instance->size() ; ++i){
		for (unsigned int j=0 ;j<actualAction->getParameters()->size();j++) {
			actualAction->getParameters()->at(j).changeName(all_instance->at(i).at(j).getName());
		}
		ret->push_back(actualAction);
		actualAction = new DurativeAction(*action);
	}
	for (vector<DurativeAction *>::iterator it = ret->begin() ; it != ret->end() ; ++it){
	}
	instance.clear();
	all_instance->clear();
	return ret;
	
}

//retrun the first plan of the graph 
Vertex * Graph::generateGraph() {
	cout<<"debut instanciations \n";
	vector<DurativeAction *> * m_actions = instanceActions();	
	cout<<"fin instanciations \n";
	DurativeAction *goalsAction =  make_actionGoal() ;
	DurativeAction *initAction =  make_actionInit();
	vector<Fluent >* lastlFlu = new vector<Fluent >();
	vector<Fluent >* actualFlu = new vector<Fluent >();
	for(unsigned i=0; i< initAction->getEffectsF().size();  ++i){
		actualFlu->push_back(*(initAction->getEffectsF().at(i)));
	}
	Vertex * firtVertex = new Vertex();
	Vertex * lastVertex = new Vertex();
	Vertex * actualVertex = new Vertex();
	Edge * edge;
	firtVertex->addAction(initAction);
	lastVertex = firtVertex;
	int plan=0;
	cout<<"generation du graph :\n";
	
	bool goal = false;
	while( !goal ){
		cout<<"plan "<<plan<<"\n";
		lastVertex = actualVertex;
		actualVertex = new Vertex();

		lastlFlu=actualFlu;
		if (actionUsable(goalsAction,lastlFlu)){
			actualVertex->addAction(goalsAction);
			for(unsigned i=0; i<goalsAction->getPreconditions().size();  ++i){			
					edge = new Edge(goalsAction->getPreconditions().at(i),lastVertex,actualVertex);
					lastVertex->addEdge(edge);
					actualVertex->addEdge(edge);
			}
			goal = true;
		} else {
		//if any action can be engage with actul fluent we add it to actual vertex
		for (vector<DurativeAction *>::iterator it_act = m_actions->begin(); it_act != m_actions->end();++it_act){
			if (actionUsable((*it_act),lastlFlu)){
				actualVertex->addAction(*it_act);
				for(unsigned i=0; i< (*it_act)->getEffectsF().size();  ++i){
					if (! compareFVF(actualFlu,(*it_act)->getEffectsF().at(i))) {
						actualFlu->push_back(*(*it_act)->getEffectsF().at(i));
					}
				}
				for(unsigned i=0; i< (*it_act)->getPreconditions().size();  ++i){			
					edge = new Edge((*it_act)->getPreconditions().at(i),lastVertex,actualVertex);
					lastVertex->addEdge(edge);
					actualVertex->addEdge(edge);
				}
			}
		}
		}
	plan++;
	}
	cout<<"generation graph end\n";
	return firtVertex;
} 


//true if the action(which is already instanciated) preconditions are satisfiable by the fluents
bool Graph::actionUsable(DurativeAction *action, vector< Fluent > * fluents){
	//each precondition need to be in fluent list
	bool c;
	for (unsigned i=0;i<action->getPreconditions().size();++i){
		c = false;	
		for (vector<Fluent>::iterator it_flu = fluents->begin() ; it_flu != fluents->end() ; ++it_flu){
			if ((*it_flu).getPredicate()->getName() == action->getPreconditions().at(i)->getPredicate()->getName()){
				if (compareVV((*it_flu).getMembersList(),action->getPreconditions().at(i)->getMembersList())){
					c = true;
				}
			}
		}
		if (!c) {
			return false;	
		}	
	}
	return true;
}	

//true if the two vector have the same members 
bool Graph::compareVV(vector<Member * >* v1 ,vector<Member * >*v2){
	if (v1->size () != v2->size()){
		return false;
	}
	for(vector<Member * >::iterator it_member = v1->begin(); it_member != v1->end() ; ++it_member){
		if ( ! compareFV( v2,(*it_member))){
			return false;
		}		
	}
	return true;
}

//true if f is in v
bool Graph::compareFVF(vector<Fluent  >* v,Fluent * f){
	for(unsigned i =0 ; i<v->size();++i){
		if (f->getPredicate()->getName() == v->at(i).getPredicate()->getName()){
			if (compareVV(f->getMembersList(),v->at(i).getMembersList() )){
				return true;
			}
		}
	}
	return false;
}

//true if m is in v
bool Graph::compareFV(vector<Member * >* v,Member * m){
	for(unsigned j =0 ; j<v->size();++j){
		if (m->getName() == v->at(j)->getName()){
			return true;
		}
	}
	return false;
}

//return an action which represent all the goal fluents ( only preconditions)
DurativeAction * Graph::make_actionGoal(){
	DurativeAction * goal = new DurativeAction((string)"Goals");
	for(unsigned i=0;i<m_problemptr->getGoals()->size();++i){
		goal->addCondition(m_problemptr->getGoals()->at(i).second,m_problemptr->getGoals()->at(i).first);
	}
	return goal;
}

//return an action which represent all the inits fluents ( only effects)
DurativeAction * Graph::make_actionInit(){
	DurativeAction * init = new DurativeAction((string)"Inits");
	for(unsigned i=0;i<m_problemptr->getInits()->size();++i){
		init->addEffect(m_problemptr->getInits()->at(i).second,m_problemptr->getInits()->at(i).first);
	}
	return init;
}
