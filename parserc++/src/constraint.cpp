/*
 * constraint.cpp
 *
 *  Created on: Apr 18, 2014
 *      Author: nathan
 */

#include "constraint.h"

namespace std {

constraint::constraint(int timeLeft, int timeRight, string comparison, DurativeAction *actionLeft, DurativeAction *actionRight) {
	// TODO Auto-generated constructor stub
	m_timeLeft = timeLeft;
	m_timeRight = timeRight;
	m_comparison = comparison;
	m_actionLeft = actionLeft;
	m_actionRight = actionRight;
	m_constraints = vector<constraint>();
}

constraint::~constraint() {
	// TODO Auto-generated destructor stub
}

void constraint::print(){
	cout << "constraint: " << m_actionLeft->getName()<<"+"<<m_timeLeft<<" "<<m_comparison<<" "<< m_actionRight->getName()<<"+"<<m_timeRight<<endl;
}

} /* namespace std */
