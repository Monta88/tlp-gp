/*
 * constraint.h
 *
 *  Created on: Apr 18, 2014
 *      Author: nathan
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

class constraint {
public:
	constraint(int timeLeft, int timeRight, string comparison, DurativeAction *actionLeft, DurativeAction *actionRight);
	virtual ~constraint();
	void print();
private:
	int m_timeLeft,m_timeRight;
	string m_comparison;
	DurativeAction *m_actionLeft, *m_actionRight;
	vector<constraint> m_constraints;
};

} /* namespace std */

#endif /* CONSTRAINT_H_ */
