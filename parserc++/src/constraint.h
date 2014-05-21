/*!
 * \file constraint.h
 * \brief [wip][test] contains things which may be used in tlpgp1
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date Apr 18, 2014
 */


#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "../Data.h"
#include "problem.h"
#include "domain.h"

namespace std {

class Constraint {
public:
	Constraint(string nameLeft, float timeLeft, string comparison, string nameRight, float timeRight);
	Constraint();
	virtual ~Constraint();
	void print();
	const string& getComparison() const;
	const string& getNameLeft() const;
	const string& getNameRight() const;
	int getTimeLeft() const;
	int getTimeRight() const;

private:
	float m_timeLeft,m_timeRight;
	string m_comparison,m_nameLeft,m_nameRight;
	DurativeAction *m_actionLeft, *m_actionRight;
	vector<Constraint> m_constraints;
};

} /* namespace std */

#endif /* CONSTRAINT_H_ */
