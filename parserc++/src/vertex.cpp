#include "vertex.h"
#include <iostream>
Vertex::Vertex(Vertex * father):m_father(father){
	m_action = new vector<DurativeAction *>();
}
	
void Vertex::addAction(DurativeAction * action){ 
	m_action->push_back(action);
}
	
Vertex::~Vertex(){}

vector<DurativeAction *> * Vertex::getActions(){
	return m_action;
}

void Vertex::to_string(){
	cout<<"\n*********plan************\nAction :\n ";
	Vertex * vertex = new Vertex(this);
	int state=0;
	do{	
		vertex = vertex->getFather();
		cout<<"\nplan : "<<state<<"nbaction : "<<vertex->getActions()->size()<<"\n";
		for(vector<DurativeAction *>::iterator it = vertex->getActions()->begin() ; it != vertex->getActions()->end() ; ++it){
			cout<<"action : "<<(*it)->to_string()/*to_string()*/<<"\n";
		}
		state++;
	} while( vertex->getFather() != NULL );
	cout<<"\n****************************\n";
}

Vertex * Vertex::getFather(){
	return m_father;
}
