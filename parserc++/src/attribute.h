#pragma once


#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>
#include "interval.h"

class Attribute {
	public:
		Attribute();
		virtual ~Attribute();
		void addForbiddens(Interval inter);
		void addNotForbiddens(Interval inter);
		void addSupported(Interval inter);
		void addNotSupported(Interval inter);
		vector<Interval> * getForbidden();
		vector<Interval> * getSupported();
		vector<Interval> * getNotForbidden();
		vector<Interval> * getNotSupported();
		string to_string();
	private:
		vector<Interval> m_forbidden;
		vector<Interval> m_supported;
		vector<Interval> m_not_forbidden;
		vector<Interval> m_not_supported;
};

#endif // ATTRIBUTE_H

		
