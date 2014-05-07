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
#include <algorithm>
#include "../Data.h"
#include "problem.h"
#include "domain.h"
#include "edge.h"
#include "vertex.h"
#include "tools.h"
#include "lObjType.h"
#include "constraint.h"
#include "sat.h"
#include "graph2.h"

using namespace std;

class Tlpgp1 {
public:
	Tlpgp1();
	Tlpgp1(Domain *domain, Problem *problem);
	virtual ~Tlpgp1();

	void constructGraph();
	void vertexToActions();

	private:
		Domain * m_domainptr;
		Problem * m_problemptr;
		Vertex *m_vertex;
		vector<vector<DurativeAction* >> m_graph;
};

#endif /* TLPGP1_H_ */
