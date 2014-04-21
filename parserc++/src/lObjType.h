#pragma once


#ifndef lObjType_H
#define lObjType_H

#include <string>
#include <vector>
#include "type.h"
#include "object.h"
using namespace std;


class lObjType {
	public:
		lObjType();
		lObjType(vector<Type*> type);
		void addObject(Object * object);
		vector<Object *> getObject();
		vector<Type*> getType();
		virtual ~lObjType();
	private:
		vector<Object *>  m_objects;
		vector<Type*>  m_type;
};

#endif // lObjType_H

