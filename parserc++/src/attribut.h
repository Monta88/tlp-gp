#pragma once


#ifndef ATTRIBUT_H
#define ATTRIBUT_H

#include <string>
#include "interval.h"

class Attribut {
	public:
		Attribut();
		virtual ~Attribut();
		void addForbidens(Interval inter);
		void addNotForbidens(Interval inter);
		void addSupported(Interval inter);
		void addNotSupported(Interval inter);
		string to_string();
	private:
		Interval m_forbidens;
		Interval m_supporteds;
		Interval m_not_forbidens;
		Interval m_not_supporteds;
};

#endif // ATTRIBUT_H

		
