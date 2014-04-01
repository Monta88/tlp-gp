#include "attribut.h"

Attribut::Attribut(){

}

Attribut::~Attribut(){

}


void Attribut::addForbidens(Interval inter){
	m_forbidens=inter;
}

void Attribut::addNotForbidens(Interval inter){
	m_not_forbidens=inter;
}
void Attribut::addSupported(Interval inter){
	m_supporteds=inter;
}
void Attribut::addNotSupported(Interval inter){
	m_not_supporteds=inter;
}

string Attribut::to_string(){
	return " à faire";
}
