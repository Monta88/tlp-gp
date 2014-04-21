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
#include "edge.h"
#include "vertex.h"
#include "tools.h"
#include "lObjType.h"
#include "constraint.h"
#include "sat.h"

using namespace std;

class Graph {
public:
	Graph();
	Graph(Domain *domain, Problem * problem);
	virtual ~Graph();
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


#endif /* GRAPH_H_ */
