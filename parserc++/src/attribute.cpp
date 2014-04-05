#include "attribute.h"
#include <sstream>
Attribute::Attribute(){

}

Attribute::~Attribute(){

}


void Attribute::addForbidens(Interval inter){
	m_forbiden=inter;
}

void Attribute::addNotForbidens(Interval inter){
	m_not_forbiden=inter;
}
void Attribute::addSupported(Interval inter){
	m_supported=inter;
}
void Attribute::addNotSupported(Interval inter){
	m_not_supported=inter;
}

template <typename T> string tostr(const T& t) { 
ostringstream os; 
os<<t; 
return os.str(); } 

string Attribute::to_string(){
	string rep="";
	if (m_forbiden.getStart() != -1.0 ){
		rep +=" forbiden on "+tostr(m_forbiden.getStart())+" - "+tostr(m_forbiden.getEnd());
	}
	if (m_supported.getStart() != -1.0 ){
		rep +=" supported on "+tostr(m_supported.getStart())+" - "+tostr(m_supported.getEnd());
	}
	if (m_not_forbiden.getStart() != -1.0 ){
		rep +=" not forbiden on "+tostr(m_not_forbiden.getStart())+" - "+tostr(m_not_forbiden.getEnd());
	}
	if (m_not_supported.getStart() != -1.0 ){
		rep +=" not supported on "+tostr(m_not_supported.getStart())+" - "+tostr(m_not_supported.getEnd());
	}
	return rep;
}
