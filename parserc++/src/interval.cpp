#include "interval.h"


Interval::Interval(float start,float end):m_start(start),m_end(end) {}

Interval::~Interval() {}

void Interval::add_argu(Variable * argu) {
	m_arguments.push_back(argu);
}

