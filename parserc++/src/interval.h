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
		Interval(float start,float end);
		virtual ~Interval();   
		void add_argu(Variable * argu);
	protected:
		float m_start;
		float m_end;
		vector <Variable*> m_arguments;
	private:
};

#endif // INTERVAL_H

