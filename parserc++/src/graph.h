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

using namespace std;

class Graph {
public:
	Graph();
	Graph(Data *data);
	virtual ~Graph();

	void generateGraph();
	bool actionUsable(DurativeAction *action, vector<pair<Fluent*, Attribute> > var);
private:
	Data *m_dataptr;
};


#endif /* GRAPH_H_ */
