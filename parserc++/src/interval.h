#pragma once


#ifndef INTERVAL_H
#define INTERVAL_H

#include <string>

using namespace std;

// If the fluent is "at a" then start=a and end=a
class Interval
{
	public:
		Interval();
		Interval(float start,float end);
		virtual ~Interval();
		float getStart();
		float getEnd();
		string to_string();
	protected:
		float m_start;
		float m_end;
};

#endif // INTERVAL_H

