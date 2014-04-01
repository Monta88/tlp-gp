#include "interval.h"

Interval::Interval(){

}
Interval::Interval(float start,float end):m_start(start),m_end(end) {}

Interval::~Interval() {}

float Interval::getStart(){
	return m_start;
}

float Interval::geEnd(){
	return m_end;
}	

