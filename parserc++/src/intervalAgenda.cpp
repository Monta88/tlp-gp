/*!
 * \file intervalAgenda.cpp
 * \brief The class represents what we will put in the tlpgp1's agenda
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 21th 2014
 */

#include "intervalAgenda.h"

using namespace std;

IntervalAgenda::IntervalAgenda(string leftAction, float timeLeft){
	// TODO Auto-generated constructor stub
	m_leftBracket = '[';
	m_rightBracket = ']';
	m_leftAction = leftAction;
	m_rightAction = "";
	m_timeLeft = timeLeft;
	m_timeRight = 0;
}

IntervalAgenda::IntervalAgenda(char leftBracket, string leftAction, float timeLeft, string rightAction, float timeRight, char rightBracket) {
	// TODO Auto-generated constructor stub
	m_leftBracket = leftBracket;
	m_rightBracket = rightBracket;
	m_leftAction = leftAction;
	m_rightAction = rightAction;
	m_timeLeft = timeLeft;
	m_timeRight = timeRight;
}

IntervalAgenda::~IntervalAgenda() {
	// TODO Auto-generated destructor stub
}

string IntervalAgenda::toString() {
	string res = "";
	if(m_rightAction != "")
		res += m_leftBracket+" "+m_leftAction+" "+to_string(m_timeLeft)+" ; "+m_rightAction+" "+to_string(m_timeRight)+ " "+m_rightBracket;
	else
		res += m_leftBracket+" "+m_leftAction+" "+to_string(m_timeLeft)+m_rightBracket;
	return res;
}

