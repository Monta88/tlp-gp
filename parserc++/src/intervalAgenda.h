/*!
 * \file intervalAgenda.h
 * \brief The class represents what we will put in the tlpgp1's agenda
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 21th 2014
 */

#ifndef INTERVALAGENDA_H_
#define INTERVALAGENDA_H_

#include <string>

using namespace std;

/*
 *
 */
class IntervalAgenda {
public:
	IntervalAgenda(string leftAction, float timeLeft);
	IntervalAgenda(char leftBracket, string leftAction, float timeLeft, string rightAction, float timeRight, char rightBracket);
	virtual ~IntervalAgenda();
	string toString();

	const string& getLeftAction() const {
		return m_leftAction;
	}

	char getLeftBracket() const {
		return m_leftBracket;
	}

	const string& getRightAction() const {
		return m_rightAction;
	}

	char getRightBracket() const {
		return m_rightBracket;
	}

	float getTimeLeft() const {
		return m_timeLeft;
	}

	float getTimeRight() const {
		return m_timeRight;
	}

private:
	char m_leftBracket, m_rightBracket;
	string m_leftAction, m_rightAction;
	float m_timeLeft, m_timeRight;
};



#endif /* INTERVALAGENDA_H_ */
