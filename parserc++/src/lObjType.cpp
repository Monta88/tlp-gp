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

string lObjType::to_string(){
	string ret="lObjType : \n \tObject :\n";
	for(vector<Object *>::iterator it = m_objects.begin() ; it != m_objects.end() ; ++it){
		ret+="\t\t "+(*it)->getName()+"\n";
	}
	return ret;
}
