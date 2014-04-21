#ifndef EDGE_H_
#define EDGE_H_

#include <vector>
#include "fluent.h"
#include "vertex.h"

using namespace std;
class Vertex;
class Edge {
	public:
		Edge();
		Edge(Fluent * fluent,Vertex * bot,Vertex * top);
		virtual ~Edge();

	private:
		Fluent * m_fluent;
		Vertex * m_bot;
		Vertex * m_top;
};


#endif /* EDGE_H_ */
