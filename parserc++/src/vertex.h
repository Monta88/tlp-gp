#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include "durative_action.h"
#include "edge.h"

using namespace std;
class Edge;
class Vertex {
public:
	Vertex();
	void addAction(DurativeAction * action);
	void addEdget(Edge * edge);
	void addEdgeb(Edge * edge);
	void to_string();
	vector<Edge *> * getEdgest();
	vector<Edge *> * getEdgesb();
	vector<DurativeAction *> * getActions();
	virtual~Vertex();

private:
	vector<DurativeAction *> * m_action;
	vector<Edge *> * m_edgesb;
	vector<Edge *> * m_edgest;
};


#endif /* VERTEX_H_ */
