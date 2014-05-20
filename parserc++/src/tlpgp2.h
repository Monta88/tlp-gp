#ifndef TLPGP2_H_
#define TLPGP2_H_

#include <iostream>
#include <ostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "vertex.h"
#include "edge.h"
#include "attribute.h"
#include "tools.h"

//using namespace std;

class Tlpgp2 {
public:	
	Tlpgp2();
	Tlpgp2(Vertex * vertex);
	virtual ~Tlpgp2();
	string generateGraphSmt2();
	string namelinkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state);
	string linkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state);
	string protectCond(string link,Fluent * fluent,Attribute att,string nameb,string namet);
	bool compareVV(vector<Member * >* v1 ,vector<Member * >*v2);
	string protectEffect(string name,DurativeAction * a,Fluent * fluent,Attribute att,int statep);
	void actionsNeeded(Fluent * fluent ,Attribute att, Vertex * vertex, Vertex  rvertex);
	void realGraph();
private:
	Vertex * m_graph;
	Vertex * m_realGraph;
	int m_nbClause;
	int m_nbAction ;
	
		
};


#endif /* TLPGP2_H_ */
