#include "edge.h"

Edge::Edge(){

}

Edge::Edge(Fluent * fluent,Vertex * bot,Vertex * top,DurativeAction * ab,DurativeAction * at):m_fluent(fluent),m_bot(bot),m_top(top),m_actionb(ab),m_actiont(at){

}

Edge::~Edge(){

}
