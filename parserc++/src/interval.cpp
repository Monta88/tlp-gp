#include "interval.h"


Interval::Interval():m_start(-1.0),m_end(-1.0){}

Interval::Interval(float start,float end):m_start(start),m_end(end) {}

Interval::~Interval() {}

float Interval::getStart(){
	return m_start;
}

float Interval::getEnd(){
	return m_end;
}

string Interval::to_string() {
	string rep = "[";
	rep += m_start;
	rep += "; ";
	rep += m_end;
	rep += "]";
	return rep;
}

