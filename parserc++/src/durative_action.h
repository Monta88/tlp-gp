#pragma once


#ifndef DURATIVE_ACTION_H
#define DURATIVE_ACTION_H

#include <string>
#include "action.h"

using namespace std;


class Duractive_Action:Action {
	public:
		Duractive_Action(string name,float duration);        
		virtual ~Duractive_Action();
	protected:
		float m_duration;
	private:
};

#endif // DURATIVE_ACTION_H

