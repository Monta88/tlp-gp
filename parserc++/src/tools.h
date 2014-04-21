#pragma once


#ifndef TOOLS_H
#define TOOLS_H

#include "type.h"
#include "fluent.h"
using namespace std;


class Tools {
	public:
		Tools();
		virtual ~Tools();
		bool isIn(Type* t ,vector<Type*>* v);
		bool compareVectorType(vector<Type*>* v1,vector<Type*>* v2);
};

#endif // TOOLS_H

