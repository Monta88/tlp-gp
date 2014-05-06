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
	vector<lObjType> * templObj = m_domainptr->getConstant();
	for (vector<lObjType>::iterator it = templObj->begin() ;it != templObj->end() ; ++it){
		objects->push_back(*it);
	}
	vector<DurativeAction * >* ret = new vector<DurativeAction *>();	
	vector<DurativeAction *>* temp ;
	vector<vector<Object * > >*objects_need = new vector<vector<Object * > >();
	vector<Object * > temp2,temp4;
	vector<Type * > temp3;
	Tools tool =Tools();
	bool find;
	for (vector<DurativeAction *>::iterator it_act = m_actions->begin(); it_act != m_actions-> end() ;++it_act){
		//build list object the action need
		for(vector<Variable >::iterator it_param = (*it_act)->getParameters()->begin() ; it_param != (*it_act)->getParameters()->end() ; ++it_param){		temp2 = vector<Object *>();
			find = false;
			for (vector<lObjType>::iterator it=objects->begin(); it != objects->end(); ++it){
				temp3 =((*it).getType());	
				if (tool.compareVectorType(&temp3,(*it_param).getTypes())) {
					temp4=(*it).getObject();
					for(vector<Object *>::iterator it_o = temp4.begin() ; it_o != temp4.end() ; ++it_o){
						temp2.push_back(*it_o);
					}
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
		if (objects_need->size() == (*it_act)->getParameters()->size()){
			temp = instanciation(objects_need,(*it_act));
		} else {
			temp = new vector<DurativeAction *>();
		}
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

// all the objects must be usefull  , return all the action instanciated with all the objects combinaisons for one action
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
bool Graph::generateGraph() {
	cout<<"debut instanciations \n";
	vector<DurativeAction *> * m_actions = instanceActions();
	cout<<"fin instanciations \n";
	DurativeAction *goalsAction =  make_actionGoal() ;cout<<" goal "<<goalsAction->to_string()<<"\n";
	DurativeAction *initAction =  make_actionInit();
	vector<Fluent >* lastlFlu = new vector<Fluent >();
	vector<Fluent >* actualFlu = new vector<Fluent >();
	for(unsigned i=0; i< initAction->getEffectsF().size();  ++i){
		actualFlu->push_back(*(initAction->getEffectsF().at(i)));
	}
	Vertex * lastVertex = new Vertex(NULL);
	Vertex * actualVertex  = new Vertex(NULL);
	actualVertex->addAction(initAction);
	int plan=0;
	cout<<"generation du graph :\n";
	Tlpgp2 tlpgp2;
	Tools t;
	bool goal = false;
	while(  !goal ){
		lastVertex = actualVertex;
		actualVertex = new Vertex(lastVertex);
		for(vector<DurativeAction *>::iterator it = lastVertex->getActions()->begin();it != lastVertex->getActions()->end(); ++it){
			actualVertex->addAction(*it);
		}
		for(unsigned int i = 0 ; i < actualFlu->size() ; ++i){
			lastlFlu->push_back(actualFlu->at(i));
		}
		actualFlu = new vector<Fluent >();
		if (actionUsable(goalsAction,lastlFlu)){
			actualVertex->addAction(goalsAction);
			cout<<"generation graph end with "<<plan<<" plan "<<endl;
			tlpgp2 = Tlpgp2(actualVertex);
			tlpgp2.generateGraphSmt2();
			t = Tools();
			if (t.solveur()){
				cout<<"succes\n";
				return true;
			}
			cout<<"fail\n";
		} 
		//if any action can be engage with actul fluent we add it to actual vertex
		for (unsigned j = 0 ; j < m_actions->size(); ++j){
			if (actionUsable(m_actions->at(j),lastlFlu)){
				if (! compareAA(actualVertex->getActions(),m_actions->at(j))){
					actualVertex->addAction(m_actions->at(j));
					for(unsigned i=0; i< m_actions->at(j)->getEffectsF().size();  ++i){
						if (! compareFVF(actualFlu,m_actions->at(j)->getEffectsF().at(i))) {
							actualFlu->push_back(*m_actions->at(j)->getEffectsF().at(i));
						}
					}
				}
			}
		}
		if((actualVertex->getActions()->size() - lastVertex->getActions()->size()) == 0){
			//actualVertex->to_string();
			cout<<"goal can't be access\n";
			goal =true;	
		}
	plan++;
	}
	return false;
} 


//true if the action(which is already instanciated) preconditions are satisfiable by the fluents
bool Graph::actionUsable(DurativeAction *action, vector< Fluent > * fluents){
	//each precondition need to be in fluent list
	bool c;
	for (unsigned i=0;i<action->getPreconditions().size();++i){
		c = false;	
		for (unsigned j = 0 ; j < fluents->size();++j){
			//if (action->getName() == "ColorFeeder-Feed-Letter-0
			if (fluents->at(j).getPredicate()->getName() == action->getPreconditions().at(i)->getPredicate()->getName()){
				if (compareVV(fluents->at(j).getMembersList(),action->getPreconditions().at(i)->getMembersList())){
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
	for(unsigned i = 0 ; i < v1->size(); ++i ){
		if ( ! (v1->at(i)->getName() == v2->at(i)->getName())){
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

//true is a is in v
bool Graph::compareAA(vector<DurativeAction *>* v,DurativeAction * a){
	for(vector<DurativeAction *>::iterator it = v->begin() ; it != v->end() ; ++it){
		if ((*it)->getName() == a->getName() ){
			if ( compareVV2((*it)->getParameters(),a->getParameters())){
				return true;
			}
		}
	}
	return false;
}

bool Graph::compareVV2(vector<Variable  >* v1 ,vector<Variable >*v2){
	if (v1->size () != v2->size()){
		return false;
	}
	for(unsigned i = 0 ; i < v1->size(); ++i ){
		if ( ! (v1->at(i).getName() == v2->at(i).getName())){
			return false;
		}		
	}
	return true;
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

//find action in a plan which can create a fluent
DurativeAction * Graph::findAction(Vertex * v,DurativeAction * initAction,Fluent * f){
	for (unsigned i = 0 ; i < v->getActions()->size() ; ++i ){
		if (compareFVF2(v->getActions()->at(i)->getEffectsF(),f)){
			return v->getActions()->at(i);
		}
	}
	if (compareFVF2(initAction->getEffectsF(),f)){
			return initAction;
	}
	return new DurativeAction("can't do this");
}

bool Graph::compareFVF2(vector<Fluent  *> v,Fluent * f){
	for(unsigned i =0 ; i<v.size();++i){
		if (f->getPredicate()->getName() == v.at(i)->getPredicate()->getName()){
			if (compareVV(f->getMembersList(),v.at(i)->getMembersList() )){
				return true;
			}
		}
	}
	return false;
}


