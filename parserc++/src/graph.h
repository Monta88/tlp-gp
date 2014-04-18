/*
 * Graph.h
 *
 *  Created on: Apr 9, 2014
 *      Author: nathan
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "../Data.h"
#include "problem.h"
#include "domain.h"
#include "constraint.h"

using namespace std;

class Graph {
public:
	Graph();
	Graph(Domain *domain, Problem *problem);
	virtual ~Graph();

	void generateGraph();
	bool actionUsable(DurativeAction *action, vector<pair<Attribute, Fluent*> > *var);
	pair< vector<DurativeAction*>, vector<pair<Attribute, Fluent*>> > nextLevel(vector<DurativeAction*> *actions,vector<pair<Attribute, Fluent*> >);
private:
	Domain *m_domainptr;
	Problem *m_problemptr;
};


#endif /* GRAPH_H_ */
