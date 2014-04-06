#include "interval.h"
#include <sstream>

Interval::Interval():m_start(-1.0),m_end(-1.0){}

Interval::Interval(float start,float end):m_start(start),m_end(end) {}

Interval::~Interval() {}

float Interval::getStart(){
	return m_start;
}

float Interval::getEnd(){
	return m_end;
}

template <typename T> string tostr(const T& t) { 
ostringstream os; 
os<<t; 
return os.str(); } 

string Interval::to_string() {
	return "["+tostr(m_start)+";"+tostr(m_end)+"]" ;
}

