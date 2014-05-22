/*
 * Graph.cpp
 *
 *  Created on: Apr 9, 2014
 */

#include "graph2.h"


Graph2::Graph2(Domain *domain, Problem *problem):m_domainptr(domain), m_problemptr(problem){}

Graph2::Graph2() {
	// TODO Auto-generated constructor stub
	m_domainptr = nullptr;
	m_problemptr = nullptr;
}

Graph2::~Graph2() {
	// TODO Auto-generated destructor stub
}

//return all the instanciation with all the objects combinaison
vector<DurativeAction * > * Graph2::instanceActions(){
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
	//for each action's domain
	for (vector<DurativeAction *>::iterator it_act = m_actions->begin(); it_act != m_actions-> end() ;++it_act){
		if ((*it_act)->getParameters()->size() != 0){
			// first build the list object the action need
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
				// second instanciate the actions with the list object
				temp = instanciation(objects_need,(*it_act));
			} else {
				//if any object of one type which is need by a action doesn't exist
				temp = new vector<DurativeAction *>();
			}
			for (vector<DurativeAction *>::iterator it_ins = temp->begin(); it_ins != temp->end() ; ++it_ins){
				ret->push_back((*it_ins));
			}
		
			objects_need->clear();
			temp->clear();
			temp2.clear();
			temp3.clear();
		} else {
			ret->push_back(*it_act);
		}
	}
	return ret;
}

// all the objects must be usefull  , return all the action instanciated with all the objects combinaisons for one action
vector<DurativeAction *>* Graph2::instanciation(vector<vector<Object * > > * objects,DurativeAction * action){
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
	/*step 2 : logically apply each object at each paramaters 
	We just need to replace the name because each paramaters's fluent point on paramaters
	*/
	for(unsigned i=0;i<all_instance->size() ; ++i){
		for (unsigned int j=0 ;j<actualAction->getParameters()->size();j++) {
			actualAction->getParameters()->at(j).changeName(all_instance->at(i).at(j).getName());
		}
		ret->push_back(actualAction);
		actualAction = new DurativeAction(*action);
	}
	instance.clear();
	all_instance->clear();
	return ret;
	
}

//retrun the first plan of the graph 
bool Graph2::generateGraph() {
	cout<<"start instanciations \n";
	vector<DurativeAction *> * m_actions = instanceActions();
	cout<<"end instanciations, "<<m_actions->size()<< "instances"<<endl;
	DurativeAction *goalsAction =  make_actionGoal() ;
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
	int p;
	/*The fluents vector (actualFlu) have all the fluent which are on effects 
	 of fluents which are on a plane
	 The first plan just have the init action .
	 The other plan are consitituite with all actions whose have their conditions satisfy 
	 by the fluents present on the vector.
	 The first action we test is the goal action and if it satisfy the graph is finish, then the last plan just avec goalsAction
	*/
	while(  !goal ){
		lastVertex = actualVertex;
		actualVertex = new Vertex(lastVertex);
		for(unsigned int i = 0 ; i < actualFlu->size() ; ++i){
			lastlFlu->push_back(actualFlu->at(i));
		}
		actualFlu = new vector<Fluent >();
		// if we can use actiongoal we stop
		if (actionUsable(goalsAction,lastlFlu)){
			actualVertex->addAction(goalsAction);
			cout<<"generation graph end with "<<plan<<" plan "<<endl;
			tlpgp2 = Tlpgp2(actualVertex);
			tlpgp2.generateSmt2();
			t = Tools();
			if (t.solveur()){
				cout<<"succes\n";
				return true;
			} 
			cout<<"fail\n";
			return true;//have to return false to retry with more plan , don't forget to cancel the last plan
		} 
		//if any action can be engage with actaul fluent we add it to the current plan(vertex)
		for (unsigned j = 0 ; j < m_actions->size(); ++j){
			if (actionUsable(m_actions->at(j),lastlFlu)){
				actualVertex->addAction(m_actions->at(j));
				for(unsigned i=0; i< m_actions->at(j)->getEffectsF().size();  ++i){
					if (! compareFVF(actualFlu,m_actions->at(j)->getEffectsF().at(i))) {
						actualFlu->push_back(*m_actions->at(j)->getEffectsF().at(i));
					}
				}
			}
		}
		if (plan == 0 ){
			p = 1;
		} else {
			p =0;
		}
		if(((actualVertex->getActions()->size() + p ) - lastVertex->getActions()->size()) == 0){
			cout<<"goal can't be access"<<lastlFlu->size()<<"\n";
			goal =true;	
		}
	plan++;
	}
	return false;
} 


//true if the action(which is already instanciated) preconditions are satisfiable by the fluents
bool Graph2::actionUsable(DurativeAction *action, vector< Fluent > * fluents){
	//each precondition need to be in fluent list
	bool c;
	Tools tool=Tools();
	for (unsigned i=0;i<action->getPreconditions().size();++i){
		c = false;	
		for (unsigned j = 0 ; j < fluents->size();++j){
			//if (action->getName() == "ColorFeeder-Feed-Letter-0
			if (fluents->at(j).getPredicate()->getName() == action->getPreconditions().at(i)->getPredicate()->getName()){
				if (tool.compareVV(fluents->at(j).getMembersList(),action->getPreconditions().at(i)->getMembersList())){
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



//true if f is in v
bool Graph2::compareFVF(vector<Fluent  >* v,Fluent * f){
	Tools tool = Tools();
	for(unsigned i =0 ; i<v->size();++i){
		if (f->getPredicate()->getName() == v->at(i).getPredicate()->getName()){
			if (tool.compareVV(f->getMembersList(),v->at(i).getMembersList() )){
				return true;
			}
		}
	}
	return false;
}


//return an action which represent all the goal fluents ( only preconditions)
DurativeAction * Graph2::make_actionGoal(){
	DurativeAction * goal = new DurativeAction((string)"Goals");
	for(unsigned i=0;i<m_problemptr->getGoals()->size();++i){
		goal->addCondition(m_problemptr->getGoals()->at(i).second,m_problemptr->getGoals()->at(i).first);
	}
	return goal;
}

//return an action which represent all the inits fluents ( only effects)
DurativeAction * Graph2::make_actionInit(){
	DurativeAction * init = new DurativeAction((string)"Inits");
	for(unsigned i=0;i<m_problemptr->getInits()->size();++i){
		init->addEffect(m_problemptr->getInits()->at(i).second,m_problemptr->getInits()->at(i).first);
	}
	return init;
}

//find action in a plan which can create a fluent
DurativeAction * Graph2::findAction(Vertex * v,DurativeAction * initAction,Fluent * f){
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

//true if f is in v
bool Graph2::compareFVF2(vector<Fluent  *> v,Fluent * f){
	Tools tool = Tools();
	for(unsigned i =0 ; i<v.size();++i){
		if (f->getPredicate()->getName() == v.at(i)->getPredicate()->getName()){
			if (tool.compareVV(f->getMembersList(),v.at(i)->getMembersList() )){
				return true;
			}
		}
	}
	return false;
}



