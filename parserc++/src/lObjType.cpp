#include "lObjType.h"

lObjType::lObjType(){
	m_objects=  vector<Object *>();
	m_type =  vector<Type *>();
}

lObjType::lObjType(vector<Type*> type):m_type(type){
	m_objects=  vector<Object *>();
}

void lObjType::addObject(Object * object){
	m_objects.push_back(object);
}
vector<Object *>  lObjType::getObject(){
	return m_objects;
}

vector<Type*>  lObjType::getType(){
	return m_type;
}
lObjType::~lObjType(){

}
