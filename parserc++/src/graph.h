/*
 * Graph.h
 *
 *  Created on: Apr 9, 2014
 *     
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
#include "attribute.h"
#include "tlpgp2.h"
#include "vertex.h"

using namespace std;

class Graph {
public:

	/*!
	 * \brief Constructor
	 */
	Graph();

	/*!
	 * \brief Constructor
	 * \param domain - action's definitions 
	 *        problem - init, goals and objects's definitions
	 */
	Graph(Domain *domain, Problem * problem);
	virtual ~Graph();

	/*!
	 * brief 
	 *retrun the first plan of the graph 
	 */
	bool generateGraph();
	
private:
	/*!
	 * brief 
	 *return the list of object sorted to instanciated the actions
	 */
	vector<DurativeAction *> * instanceActions();
	/*!
	 * brief 
	 * all the objects must be usefull  
	 * return all the action instanciated with all the objects combinaisons 
	 */
	vector<DurativeAction *>* instanciation(vector<vector<Object * > > * objects,DurativeAction *action);

	/*!
	 * brief
	 *return true if the action(which is already instanciated) preconditions are satisfiable by the fluents
	 */
	bool actionUsable(DurativeAction *action, vector< Fluent > * fluents);

	/*!
	 * brief
	 *true if fluent f is in v
	 */
	bool compareFVF(vector<Fluent  >* v,Fluent * f);

	/*!
	 * brief
	 *true if fluent f is in v
	 */
	bool compareFVF2(vector<Fluent  *> v,Fluent * f);
	
	/*!
	 * brief
	 *find action in a plan which can create a fluent
	 */
	DurativeAction * findAction(Vertex * v,DurativeAction *initAction,Fluent * f);	

	/*!
	 * brief
	 *return an action which represent all the goal fluents ( only preconditions)
	 */
	DurativeAction * make_actionInit();
	
	/*!
	 * brief
	 *return an action which represent all the inits fluents ( only effects)
	 */
	DurativeAction * make_actionGoal();
	
	Domain * m_domainptr;
	Problem * m_problemptr;
};


#endif /* GRAPH_H_ */

