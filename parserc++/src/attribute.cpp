/*!
 * \file attribute.cpp
 * \brief This class represent a PDDL Temporally Extended attribute (invented by Frederic Maris), which are the main feature of the interval of preconditions and effects of PDDL durative-actions
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#include "attribute.h"


Attribute::Attribute() {
	m_forbidden = vector<Interval> ();
	m_supported = vector<Interval> ();
	m_not_forbidden = vector<Interval> ();
	m_not_supported = vector<Interval> ();
}

Attribute::~Attribute() {}

void Attribute::addForbidden(Interval inter) {
	m_forbidden.push_back(inter);
}

void Attribute::addNotForbidden(Interval inter) {
	m_not_forbidden.push_back(inter);
}
void Attribute::addSupported(Interval inter) {
	m_supported.push_back(inter);
}
void Attribute::addNotSupported(Interval inter) {
	m_not_supported.push_back(inter);
}

float Attribute::getTime() {
	float ret = 0;
	
	// for each interval in m_supported (because of the "at")
	for(vector<Interval>::iterator it = m_supported.begin(); it != m_supported.end(); ++it) {
		if ((*it).getStart() == (*it).getEnd()) { // if the time of start and the time of end of the current interval are equals (if it is an "at")
			if ((*it).getEnd() > ret) { // we keep the maximum time
				ret = (*it).getEnd();
			}
		}
	}
	
	return ret;
}

vector<Interval> * Attribute::getForbidden() {
	return &m_forbidden;
}

vector<Interval> * Attribute::getSupported() {
	return &m_supported;
}

vector<Interval> * Attribute::getNotForbidden() {
	return &m_not_forbidden;
}

vector<Interval> * Attribute::getNotSupported() {
	return &m_not_supported;
}

string Attribute::to_string() {
	string rep="";
	if (m_forbidden.size() != 0) { // if there are interval that are forbidden
		rep +=" forbidden on";
		
		// we print each interval that is forbidden
		for (vector<Interval>::iterator it = m_forbidden.begin(); it != m_forbidden.end(); ++it) {
			rep += " " + (*it).to_string();
		}
	}
	if (m_supported.size() != 0) { // if there are interval that are supported
		rep +=" supported on";
		
		// we print each interval that is supported
		for (vector<Interval>::iterator it = m_supported.begin(); it != m_supported.end(); ++it) {
			rep += " " + (*it).to_string();
		}
	}
	if (m_not_forbidden.size() != 0) { // if there are interval that are not forbidden
		rep +=" not forbidden on";
		
		// we print each interval that is not forbidden
		for (vector<Interval>::iterator it = m_not_forbidden.begin(); it != m_not_forbidden.end(); ++it) {
			rep += " " + (*it).to_string();
		}
	}
	if (m_not_supported.size() != 0) { // if there are interval that are not supported
		rep +=" not supported on";
		
		// we print each interval that is not supported
		for (vector<Interval>::iterator it = m_not_supported.begin(); it != m_not_supported.end(); ++it) {
			rep += " " + (*it).to_string();
		}
	}
	return rep;
}
