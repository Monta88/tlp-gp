/*
 * tlpgp1.h
 *
 *  Created on: Apr 23, 2014
 *      Author: nathan
 */

#ifndef TLPGP1_H_
#define TLPGP1_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "../Data.h"
#include "problem.h"
#include "domain.h"
#include "edge.h"
#include "vertex.h"
#include "tools.h"
#include "lObjType.h"
#include "constraint.h"
#include "sat.h"

using namespace std;

class Tlpgp1 {
public:
	Tlpgp1();
	Tlpgp1(Domain *domain, Problem *problem);
	virtual ~Tlpgp1();

	vector<DurativeAction > * instanceActions();
	vector<DurativeAction >* instanciation(vector<vector<Object * > > * objects,DurativeAction *action);
	void generateGraph();
	bool actionUsable(DurativeAction *action, vector<pair<Attribute, Fluent*> > *var);
	bool actionUsable(DurativeAction action, vector< Fluent > * fluents);
	bool compareVV(vector<Member * >* v1 ,vector<Member * >*v2);
	bool compareFV(vector<Member * >* v1,Member * m);
	pair< vector<DurativeAction*>, vector<pair<Attribute, Fluent*>> > nextLevel(vector<DurativeAction*> *actions,vector<pair<Attribute, Fluent*> >);
	private:
		Domain * m_domainptr;
		Problem * m_problemptr;
};

#endif /* TLPGP1_H_ */
