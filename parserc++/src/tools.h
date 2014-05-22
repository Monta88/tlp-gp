#pragma once


#ifndef TOOLS_H
#define TOOLS_H

#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <fstream>
#include "type.h"
#include "fluent.h"
#include "durative_action.h"
using namespace std;


class Tools {
	public:
		Tools();
		virtual ~Tools();
		bool isIn(Type* t ,vector<Type*>* v);
		bool compareVectorType(vector<Type*>* v1,vector<Type*>* v2);
		bool compareActionVaction(DurativeAction * action,vector<DurativeAction * >* v);
		//true if the two vector have the same members 
		bool compareVV(vector<Member * >* v1 ,vector<Member * >*v2);
		bool compareVV2(vector<Variable  >* v1 ,vector<Variable  >*v2);
		bool compareAA(DurativeAction * a1 , DurativeAction * a2);
		
		bool solveur();
};

#endif // TOOLS_H

