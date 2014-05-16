/*!
 * \file constraint.cpp
 * \brief [wip][test] contains things which may be used in tlpgp1
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date Apr 18, 2014
 */


#include "constraint.h"

namespace std {

Constraint::Constraint() {
	// TODO Auto-generated constructor stub
	m_timeLeft = 0;
	m_timeRight = 0;
	m_comparison = "";
	m_nameLeft = "";
	m_nameRight = "";
	m_actionLeft=nullptr;
	m_actionRight=nullptr;
}

Constraint::Constraint(string nameLeft, int timeLeft, string comparison, string nameRight, int timeRight) {
	// TODO Auto-generated constructor stub
	m_timeLeft = timeLeft;
	m_timeRight = timeRight;
	m_comparison = comparison;
	m_nameLeft = nameLeft;
	m_nameRight = nameRight;
	m_actionLeft=nullptr;
	m_actionRight=nullptr;
}

Constraint::~Constraint() {
	// TODO Auto-generated destructor stub
}

void Constraint::print(){
	cout << "constraint: " << m_nameLeft << " + " <<m_timeLeft << " " <<m_comparison<< " "<< m_nameRight<< " + "<< m_timeRight <<endl;
}

const string& Constraint::getComparison() const {
	return m_comparison;
}

const string& Constraint::getNameLeft() const {
	return m_nameLeft;
}

const string& Constraint::getNameRight() const {
	return m_nameRight;
}

int Constraint::getTimeLeft() const {
	return m_timeLeft;
}

int Constraint::getTimeRight() const {
	return m_timeRight;
}

} /* namespace std */
