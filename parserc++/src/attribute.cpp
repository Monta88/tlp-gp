#include "attribute.h"

Attribute::Attribute(){

}

Attribute::~Attribute(){

}


void Attribute::addForbidens(Interval inter){
	m_forbidens=inter;
}

void Attribute::addNotForbidens(Interval inter){
	m_not_forbidens=inter;
}
void Attribute::addSupported(Interval inter){
	m_supporteds=inter;
}
void Attribute::addNotSupported(Interval inter){
	m_not_supporteds=inter;
}

string Attribute::to_string(){
	return "Attribute";
}
