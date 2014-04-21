/*
 * Graph.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: nathan
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
/*
void Graph::generateGraph() {
	vector<pair<Fluent*, Attribute> > *inits, *goals ;
	vector<DurativeAction*> *actions ;
	pair< vector<DurativeAction*>, vector<pair<Attribute, Fluent*>> > level0,level1,level2,level3;
	DurativeAction goalsAction("Goals") ;
	sat sol;

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

	vect2.clear();
	//vect2.insert(vect2.end(),inits2.begin(),inits2.end());
	//vect2 = inits2;
	cout << "\nETAPE 1: " << vect2.size() << endl;
	for(auto it_act = actions->begin(); it_act != actions->end(); ++it_act) {
		if(actionUsable(*it_act, &inits2)){
			cout << (*it_act)->getName() << endl;

			vect3 = (*it_act)->getEffects();
			vect2.insert(vect2.end(), vect3.begin(), vect3.end());
		}
	}
	cout << "RESULT: " << vect2.size() << " " << actionUsable(&goalsAction, &vect2)<< endl;


	cout << "level 0: " << " " << actionUsable(&goalsAction, &vect2)<< endl;

	level1 = nextLevel(actions,vect2);
	cout << "level 1: " << " " << actionUsable(&goalsAction, &vect2)<< endl;
	cout << ": " << level1.first.size() << " " << level1.second.size()<< endl;

	level2 = nextLevel(&level1.first,level1.second);
	cout << "level 2: " << " " << actionUsable(&goalsAction, &vect2)<< endl;
	cout << ": " << level2.first.size() << " " << level2.second.size()<< endl;

	level3 = nextLevel(&level2.first,level2.second);
	cout << "level 2: " << " " << actionUsable(&goalsAction, &vect2)<< endl;
	cout << ": " << level3.first.size() << " " << level3.second.size()<< endl;


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

	constraint c (1,2,"<",(*actions)[0],(*actions)[1]);
	c.print();

	sol.initialize();

	cout << "\n(generateGraph): END" <<endl;
}

pair< vector<DurativeAction*>, vector<pair<Attribute, Fluent*>> > Graph::nextLevel(vector<DurativeAction*> *actions,vector<pair<Attribute, Fluent*> > conditions){
	vector<pair<Attribute, Fluent*> > newEffects,tempEffects;
	vector<DurativeAction*> newActions ;
	vector<pair<Attribute, Fluent*> >::iterator pos ;
	pair< vector<DurativeAction*>, vector<pair<Attribute, Fluent*>> > result;

	cout << "\nETAPE : " << conditions.size() << endl;
	for(auto it_act = actions->begin(); it_act != actions->end(); ++it_act) {
		if(actionUsable(*it_act, &conditions)){
			//cout << (*it_act)->getName() << endl;
			newActions.push_back(*it_act);
			tempEffects = (*it_act)->getEffects() ;

			for(auto it_eff = tempEffects.begin(); it_eff != tempEffects.end(); ++it_eff){
				pos = find(newEffects.begin(), newEffects.end(), it_eff) ;
				if(pos == newEffects.end() ){
					newEffects.push_back(*it_eff);
				}
			}
			newEffects.insert(newEffects.end(),tempEffects.begin(),tempEffects.end());
		}
	}

	result.first = newActions;
	result.second = newEffects;

	return result;
}

// Given a list of fluents, return if the action is activable (ie all preconditions are satisfied)
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
}*/

vector<DurativeAction > * Graph::instanceActions(){
	vector<DurativeAction *> * m_actions = m_domainptr->getActions();
	vector<lObjType> * objects = m_problemptr->getObjects();
	vector<DurativeAction >* ret = new vector<DurativeAction >();	
	vector<DurativeAction >* temp ;
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
		for (vector<DurativeAction >::iterator it_ins = temp->begin(); it_ins != temp->end() ; ++it_ins){
			ret->push_back(*it_ins);
		}
		objects_need->clear();
		temp->clear();
		temp2.clear();
		temp3.clear();
	}
	return ret;
}

// all the objects must be usefull 
vector<DurativeAction >* Graph::instanciation(vector<vector<Object * > > * objects,DurativeAction * action){
	vector<vector<Object  > >* all_instance = new vector<vector<Object >>();
	vector<Object > instance =  vector<Object  >();
	int * tabMax = new int[objects->size()];
	int * tab = new int[objects->size()];
	string objectN,temp;
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
	cout<<"\ndebut\n"<<action->to_string()<<"\nfin\n";
	
	//step 2 create one action / combinaison
	vector<DurativeAction >* ret=new vector<DurativeAction >();
	DurativeAction actualAction = *action;
	for(vector<vector<Object  >>::iterator it_inst = all_instance->begin() ; it_inst != all_instance->end() ;++it_inst){
		actualAction=*action ;
		for (unsigned int i=0 ;i<actualAction.getParameters()->size();i++) {
			objectN = actualAction.getParameters()->at(i).getName();
			// probleme ici d'instantiation normalement il faudrai juste changer les paramtetre mais marche pas 
			actualAction.getParameters()->at(i).changeName((*it_inst).at(i).getName());
		}
		ret->push_back(actualAction);
	}
	for (vector<DurativeAction >::iterator it = ret->begin() ; it != ret->end() ; ++it){
		cout<<"\ndebut\n"<<(*it).to_string()<<"\nfin\n";
	}
	instance.clear();
	all_instance->clear();
	return ret;
	
}


void Graph::generateGraph() {
	bool goal = false;
	vector<DurativeAction > * m_actions = instanceActions();	
	DurativeAction goalsAction =  DurativeAction((string)"Goals") ;
	DurativeAction initAction =  DurativeAction("Inits") ;
	//create action goals
	for(vector<pair<Fluent*, Attribute> >::iterator it = m_problemptr->getGoals()->begin(); it != m_problemptr->getGoals()->end(); ++it){
		goalsAction.addCondition((*it).second,(*it).first);
	}
	vector<Fluent >* lastlFlu = new vector<Fluent >();
	vector<Fluent >* actualFlu = new vector<Fluent >();
	Fluent*  temp;
	//create action init
	for(vector<pair<Fluent*, Attribute> >::iterator it = m_problemptr->getInits()->begin(); it != m_problemptr->getInits()->end(); ++it){
		temp =((*it).first);		
		actualFlu->push_back(*temp);
		initAction.addCondition((*it).second,(*it).first);
	}
	
	Vertex * firtVertex = new Vertex();
	Vertex * lastVertex = new Vertex();
	Vertex * actualVertex = new Vertex();
	Edge * edge;
	firtVertex->addAction(&initAction);
	lastVertex = firtVertex;
	int plan=0;
	bool c;
	cout<<"generation du graph :\n";
	goal=true;//instantiation marche pas donc on pas generé le graphe

	while( !goal ){
		//cout<<"plan "<<plan<<"\n";
		lastVertex = actualVertex;
		actualVertex = new Vertex();

		lastlFlu=actualFlu;
		//if we can have goal we stop
		/*for (unsigned i=0;i<goalsAction.getPreconditions().size() ;++i){
			cout<<" goal "<<goalsAction.getPreconditions().at(i)->getPredicate()->getName()<<"\n";
		}
		cout<<"\n\n";
		for (unsigned i=0; i<lastlFlu->size();++i){
			
			cout<<"  list  "<<lastlFlu->at(i).getPredicate()->getName()<<"";
			for(unsigned j=0;j<lastlFlu->at(i).getMembersList()->size();j++){
				cout<<" | "<<lastlFlu->at(i).getMembersList()->at(j)->getName();
			}cout<<"\n";
		}
		cout<<"\n%%%%%\n	";*/
		if (actionUsable(goalsAction,lastlFlu)){
			actualVertex->addAction(&goalsAction);
			for(vector< Fluent *>::iterator it_flu = goalsAction.getPreconditions().begin(); it_flu != goalsAction.getPreconditions().end(); ++it_flu){
					edge = new Edge((*it_flu),lastVertex,actualVertex);
					lastVertex->addEdge(edge);
					actualVertex->addEdge(edge);
					actualFlu->push_back(*(*it_flu));
				}
			goal = true;
		} else {
		//if any action can be engage with actul fluent we add it to actual vertex
		for (vector<DurativeAction > ::iterator it_act = m_actions->begin(); it_act != m_actions->end();++it_act){
			if (actionUsable((*it_act),lastlFlu)){
				actualVertex->addAction(&(*it_act));
				for(unsigned i=0; i< (*it_act).getEffectsF().size();  ++i){			
					edge = new Edge((*it_act).getEffectsF().at(i),lastVertex,actualVertex);
					lastVertex->addEdge(edge);
					actualVertex->addEdge(edge);
					c = true;	
					for (vector<Fluent>::iterator it_flu = actualFlu->begin() ; it_flu != actualFlu->end() ; ++it_flu){
						if ((*it_flu).getPredicate()->getName() == (*it_act).getEffectsF().at(i)->getPredicate()->getName()){
							c = false;
						}
					}
					if (c) {
						actualFlu->push_back(*(*it_act).getEffectsF().at(i));
					}

				}
			}
		}
		}
	plan++;
	}
} 



bool Graph::actionUsable(DurativeAction action, vector< Fluent > * fluents){
	//each precondition need to be in fluent list
	bool c;
	for (unsigned i=0;i<action.getPreconditions().size();++i){
		c = false;	
		for (vector<Fluent>::iterator it_flu = fluents->begin() ; it_flu != fluents->end() ; ++it_flu){
			if ((*it_flu).getPredicate()->getName() == action.getPreconditions().at(i)->getPredicate()->getName()){
				if (compareVV((*it_flu).getMembersList(),action.getPreconditions().at(i)->getMembersList())){
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

bool Graph::compareVV(vector<Member * >* v1 ,vector<Member * >*v2){
	for(vector<Member * >::iterator it_member = v1->begin(); it_member != v1->end() ; ++it_member){
		if ( ! compareFV( v2,(*it_member))){
			return false;
		}		
	}
	return true;
}

bool Graph::compareFV(vector<Member * >* v,Member * m){
	for(unsigned j =0 ; j<v->size();++j){
		if (m->getName() == v->at(j)->getName()){
			return true;
		}
	}
	return false;
}
