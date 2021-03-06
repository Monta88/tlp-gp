/*!
 * \file tlpgp1.h
 * \brief Contains the core code of tlpgp1
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date Apr 23, 2014
 */

#ifndef TLPGP1_H_
#define TLPGP1_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <utility>
#include "../Data.h"
#include "problem.h"
#include "domain.h"
#include "edge.h"
#include "vertex.h"
#include "tools.h"
#include "lObjType.h"
#include "constraint.h"
#include "sat.h"
#include "graph1.h"
#include "intervalAgenda.h"

using namespace std;

class Tlpgp1 {
public:
	Tlpgp1();
	Tlpgp1(Domain *domain, Problem *problem);
	virtual ~Tlpgp1();

	/*!
	 * \brief tlpgp1 core function
	 */
	void constructGraph();

	/*!
	* \brief converts a graph into an vector of vector
	* (*father,vector of actions) -> (vector<vector action>)
	*/
	void vertexToActions();

private:
	/*!< pointer on the domain */
	Domain * m_domainptr;

	/*!< pointer on the problem */
	Problem * m_problemptr;

	/*!< graph generated by Graph2 */
	Vertex *m_vertex;

	/*!< graph2 : will be use to access some functions*/
	Graph1 m_graph2;

	/*!< graph converted by vertexToActions */
	vector<vector<DurativeAction*>> m_graph;

	Sat m_sat;

	/*!
	* \brief select an action producing a given effect at a given level
	* return the action and the level
	* if we are looking for an action which produces "e" for level "i", use sameLevel=true;
	*/
	pair<DurativeAction* , int> getActionByEffect(Fluent *effect, int level, bool sameLevel, DurativeAction* act);

	void removeGoals(Fluent *fluent, vector<Fluent*> *goals);

	void printGoals(vector<Fluent*> *goals);

	void printConstraints(vector<Constraint> *constraints);

	void declareFun(Sat *s);

	string fluentToAgendaName(Fluent *f);

	string actionToName(DurativeAction *act, int level);

	string actionNameToName(string name);

	DurativeAction* getActionByName(string name);

	// add an interval to the agenda of effectName
	bool addToAgenda(vector<pair<string, vector<IntervalAgenda>>> *agenda, string effectName, IntervalAgenda *inter);

	vector<IntervalAgenda> getIntervalsFromAgenda(vector<pair<string, vector<IntervalAgenda>>> *agenda, string effectName);
};

#endif /* TLPGP1_H_ */
