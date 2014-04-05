#pragma once


#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include "interval.h"

class Attribute {
	public:
		Attribute();
		virtual ~Attribute();
		void addForbidens(Interval inter);
		void addNotForbidens(Interval inter);
		void addSupported(Interval inter);
		void addNotSupported(Interval inter);
		string to_string();
	private:
		Interval m_forbiden;
		Interval m_supported;
		Interval m_not_forbiden;
		Interval m_not_supported;
};

#endif // ATTRIBUTE_H

		
