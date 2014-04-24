#include "vertex.h"
#include <iostream>
Vertex::Vertex(){
	m_action = new vector<DurativeAction *>();
	m_edgesb	= new vector<Edge *>();
	m_edgest	= new vector<Edge *>();
}
	
void Vertex::addAction(DurativeAction * action){ 
	m_action->push_back(action);
}
	
void Vertex::addEdgeb(Edge * edge){ 
	m_edgesb->push_back(edge);
}

void Vertex::addEdget(Edge * edge){ 
	m_edgest->push_back(edge);
}
	
Vertex::~Vertex(){}

vector<Edge *> * Vertex::getEdgest(){
	return m_edgest;
}

vector<Edge *> * Vertex::getEdgesb(){
	return m_edgesb;
}

vector<DurativeAction *> * Vertex::getActions(){
	return m_action;
}

void Vertex::to_string(){
	cout<<"\n*********plan************\nAction :\n ";
	for(vector<DurativeAction *>::iterator it = m_action->begin() ; it != m_action->end() ; ++it){
		cout<<(*it)->getName()<<"  ";
		for(vector<Variable >::iterator it2 = (*it)->getParameters()->begin() ;it2 != (*it)->getParameters()->end();++it2){
			cout<<""<<(*it2).getName()<<" | ";
		}cout<<"\n";
	}
	cout<<"\nlink  "<<m_edgest->size();
	for(vector<Edge *>::iterator it = m_edgest->begin() ; it != m_edgest->end() ; ++it){
		cout<<"		"<<(*it)->getFluent()->to_string()<<"\n";
	}

	if (m_edgest->size() != 0){	
		Vertex * actual = this;
		while(actual->getEdgest()->size() != 0){
			actual = actual->getEdgest()->at(0)->getTop();
			cout<<"\n*********plan************ "<< actual->getEdgesb()->size()<<"\nAction :		 ";
			for(vector<DurativeAction *>::iterator it = actual->getActions()->begin() ; it != actual->getActions()->end() ; ++it){
				cout<<(*it)->getName()<<"  ";
				for(vector<Variable >::iterator it2 = (*it)->getParameters()->begin() ;it2 != (*it)->getParameters()->end();++it2){
					cout<<""<<(*it2).getName()<<" | ";
				}cout<<"\n";
			}
			cout<<"\nlink";
			for(vector<Edge *>::iterator it = actual->getEdgest()->begin() ; it != actual->getEdgest()->end() ; ++it){
				cout<<"		"<<(*it)->getFluent()->to_string()<<"\n";
			}
		cout<<"\n*********"<<actual->getEdgest()->size()<<"\n";
		}
	}
}
