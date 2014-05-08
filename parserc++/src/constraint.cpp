/*!
 * \file constraint.cpp
 * \brief [wip][test] contains things which may be used in tlpgp1
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date Apr 18, 2014
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
