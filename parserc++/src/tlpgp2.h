
#ifndef TLPGP2_H_
#define TLPGP2_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "vertex.h"
#include "edge.h"
using namespace std;

class Tlpgp2 {
public:	
	Tlpgp2();
	Tlpgp2(Vertex * vertex);
	virtual ~Tlpgp2();
	string generateGraphSmt2();
	vector<pair<string,float> >  findDestroyAction(Fluent * f);
	bool isdestroy(vector<Fluent  *> v,Fluent * f);
	bool compareVV(vector<Member * >* v1 ,vector<Member * >*v2);
	float findTimeEff(Fluent * f,DurativeAction * a);
private:
	Vertex * m_graph;
		
};


#endif /* TLPGP2_H_ */
