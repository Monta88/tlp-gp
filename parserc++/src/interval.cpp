#include "interval.h"
#include <sstream>


Interval::Interval():m_start(-1.0), m_end(-1.0), m_start_bracket(true), m_end_bracket(false) {}

Interval::Interval(float start,float end):m_start(start), m_end(end), m_start_bracket(true), m_end_bracket(false) {}

Interval::Interval(float start, float end, bool start_bracket, bool end_bracket):m_start(start), m_end(end), m_start_bracket(start_bracket), m_end_bracket(end_bracket) {}

Interval::~Interval() {}

float Interval::getStart(){
	return m_start;
}

float Interval::getEnd(){
	return m_end;
}

bool Interval::getStartBracket() {
	return m_start_bracket;
}

bool Interval::getEndBracket() {
	return m_end_bracket;
}

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os<<t;
	return os.str();
} 

string Interval::to_string() {
	string str = "";
	if (m_start_bracket)
		str += "[";
	else
		str += "]";
	str += tostr(m_start) + "; " + tostr(m_end);
	if (m_end_bracket)
		str += "[";
	else
		str += "]";
	return str;
}

