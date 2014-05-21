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
	/*!
	 * \brief Constructor
	 */
	Tlpgp2();

	/*!
	 * \brief Constructor
	 * \param vertex - probleme graph  
	 */
	Tlpgp2(Vertex * vertex);


	virtual ~Tlpgp2();

	/*!
	 * brief 
	 *global method, write in a file all what a solver need to find a planification model
	 */
	string generateSmt2();

private:
	/*!
	 * brief 
	 *return  propositionals variables : links between a action(name) and the actions whose add the fluent
	 */
	string namelinkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state);

	/*!
	 * brief 
	 *return  clauses: links between a action(name) and the actions whose add the fluent
	 * "Production des préconditions par liens causaux" 
	 */
	string linkPrec(string name ,Fluent * fluent ,Attribute att, Vertex * vertex,int state);

	/*!
	 * use in linkPrec
	 * return clause : action which del a fluent which is on a link
	 * "Activation des actions et ordre partiel"
	 */
	string protectCond(string link,Fluent * fluent,Attribute att,string nameb,string namet);

	/*!
	 * return true if v1 have the same length and elements
	 */
	bool compareVV(vector<Member * >* v1 ,vector<Member * >*v2);

	/*!
	 * return clause : when a action add a fluent and another del it
	 * "Exclusions mutuelles temporellement étendues"
	 */
	string protectEffect(string name,DurativeAction * a,Fluent * fluent,Attribute att,int statep);

	/*!
	 * delete the actions which are useless to the goal
	 */
	void realGraph();

	/*!
	 * use in realGraph()
	 */
	void actionsNeeded(Fluent * fluent ,Attribute att, Vertex * vertex, Vertex  rvertex);

	Vertex * m_graph;
	Vertex * m_realGraph;
	int m_nbClause;
	int m_nbVariableProp;
	int m_nbVariableReal;
	
		
};


#endif /* TLPGP2_H_ */
