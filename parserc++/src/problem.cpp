#include "problem.h"
Problem::Problem(string name):m_name(name) {
		m_objects = new vector<Object*> ();
		m_inits = new vector<pair<Fluent*, Attribute> > ();
		m_goals = new vector<pair<Fluent*, Attribute> > ();
}

Problem::Problem() {}

Problem::~Problem() {}

string Problem::getName() {
	return m_name;
}

void Problem::addObjects(vector<Object*> * objects) {
	m_objects = objects;
}

void Problem::addInits(vector<pair<Fluent*, Attribute> > * inits) {
	m_inits = inits;
}

void Problem::addGoals(vector<pair<Fluent*, Attribute> > * goals) {
	m_goals = goals;
}

// return a list of a pair (objects type)
vector<lObjType> * Problem::getObjects(){
	Tools tools = Tools();
	vector<lObjType> * ret = new vector<lObjType>();
	vector<Type*>* types_obj,temp2;
	bool marq;
	lObjType temp;
	for (vector<Object*>::iterator it_objects=m_objects->begin(); it_objects != m_objects->end() ; ++it_objects){//each object
		types_obj= (*it_objects)->getTypes();
		if ( ret->size() != 0){
			marq=true;
			for(vector<lObjType>::iterator it_ret = ret->begin() ;  it_ret != ret->end() ; ++it_ret){
				temp2 =(*it_ret).getType();
				if (tools.compareVectorType(&temp2,types_obj)){
					(*it_ret).addObject(*it_objects);
					marq=false;
				} 
			}
			if(marq) {
				temp = lObjType(*(types_obj));
				temp.addObject(*it_objects);
				ret->push_back(temp);
			}
		} else {
			temp = lObjType(*(types_obj));
			temp.addObject(*it_objects);
			ret->push_back(temp);
		}
	}
	return ret;		
}


