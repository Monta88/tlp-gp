#pragma once


#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <iostream> 
#include <string> 
#include <vector>
#include "variable.h"
#include "interval.h"

using namespace std;


class Attribute {
	public:
		Attribute();
		virtual ~Attribute();
		void add_supported(Interval inter);
		void add_forbiden(Interval inter);
	private:
		vector<Interval> m_forbidens;
		vector<Interval> m_supporteds;
};

#endif // ATTRIBUTE_H

