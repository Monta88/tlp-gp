#include "tools.h"
#include <iostream>
Tools::Tools(){}
		
Tools::~Tools(){
	// TODO Auto-generated destructor stub
}

// return true is the 2 vector have the same types
bool Tools::compareVectorType(vector<Type*>* v1,vector<Type*>* v2){
	for (vector<Type*>::iterator it = v1->begin() ; it != v1->end() ; ++it){
		if (!isIn((*it),v2)) {
			return false;
		}
	}
	return true;
}

bool Tools::isIn(Type* t ,vector<Type*>* v){						
	for (vector<Type*>::iterator it = v->begin() ; it != v->end() ; ++it){
		if ((*it)->getName() == t->getName())	{
			return true;
		}
	}
	return false;
}

