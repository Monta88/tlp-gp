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
	void addEdge(Edge * edge);
	virtual~Vertex();

private:
	vector<DurativeAction *> * m_action;
	vector<Edge *> * m_edges;
};


#endif /* VERTEX_H_ */
