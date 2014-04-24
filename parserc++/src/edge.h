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
		Edge(Fluent * fluent,Vertex * bot,Vertex * top,DurativeAction * ab,DurativeAction * at);
		Fluent * getFluent(){
			return m_fluent;
		}
		Vertex * getTop(){
			return m_top;
		}
		DurativeAction * getActionb(){
			return m_actionb;
		}
		DurativeAction * getActiont(){
			return m_actiont;
		}
		virtual ~Edge();

	private:
		Fluent * m_fluent;
		Vertex * m_bot;
		Vertex * m_top;
		DurativeAction * m_actiont;
		DurativeAction * m_actionb;
};


#endif /* EDGE_H_ */
