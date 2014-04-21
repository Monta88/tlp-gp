#include "vertex.h"
#include <iostream>
Vertex::Vertex(){
	m_action = new vector<DurativeAction *>();
	m_edges	= new vector<Edge *>();
}
	
void Vertex::addAction(DurativeAction * action){ 
	m_action->push_back(action);
}
	
void Vertex::addEdge(Edge * edge){ 
	m_edges->push_back(edge);
}
	
Vertex::~Vertex(){}
