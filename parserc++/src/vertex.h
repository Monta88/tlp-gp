#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include "durative_action.h"

using namespace std;
class Vertex {
public:
	Vertex(Vertex * father);
	void addAction(DurativeAction * action);
	void to_string();
	vector<DurativeAction *> * getActions();
	Vertex * getFather();
	void addFather(Vertex * vertex);
	void addActions(vector<DurativeAction *> * actions);
	virtual~Vertex();

private:
	vector<DurativeAction *> * m_action;
	Vertex * m_father;
};


#endif /* VERTEX_H_ */
