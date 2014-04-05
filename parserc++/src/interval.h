#pragma once


#ifndef INTERVAL_H
#define INTERVAL_H

#include <string>
#include <vector>
#include "variable.h"

using namespace std;

//Si le fluent est produit au moment a alor start=a et end=a
class Interval
{
	public:
		Interval();
		Interval(float start,float end);
		float getStart();
		float getEnd();
		virtual ~Interval();
	protected:
		float m_start;
		float m_end;
};

#endif // INTERVAL_H

