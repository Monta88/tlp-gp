#include "attribute.h"


Attribute::Attribute() {
	m_forbidden = vector<Interval> ();
	m_supported = vector<Interval> ();
	m_not_forbidden = vector<Interval> ();
	m_not_supported = vector<Interval> ();
}

Attribute::~Attribute() {}

void Attribute::addForbiddens(Interval inter) {
	m_forbidden.push_back(inter);
}

void Attribute::addNotForbiddens(Interval inter) {
	m_not_forbidden.push_back(inter);
}
void Attribute::addSupported(Interval inter) {
	m_supported.push_back(inter);
}
void Attribute::addNotSupported(Interval inter) {
	m_not_supported.push_back(inter);
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
	if (m_forbidden.size() != 0) {
		rep +="forbidden on";
		for (vector<Interval>::iterator it = m_forbidden.begin(); it != m_forbidden.end(); ++it) {
			rep += " " + it->to_string();
		}
	}
	if (m_supported.size() != 0) {
		rep +="supported on";
		for (vector<Interval>::iterator it = m_supported.begin(); it != m_supported.end(); ++it) {
			rep += " " + it->to_string();
		}
	}
	if (m_not_forbidden.size() != 0) {
		rep +="not forbidden on";
		for (vector<Interval>::iterator it = m_not_forbidden.begin(); it != m_not_forbidden.end(); ++it) {
			rep += " " + it->to_string();
		}
	}
	if (m_not_supported.size() != 0) {
		rep +="not supported on";
		for (vector<Interval>::iterator it = m_not_supported.begin(); it != m_not_supported.end(); ++it) {
			rep += " " + it->to_string();
		}
	}
	return rep;
}
