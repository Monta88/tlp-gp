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
		Interval(float start, float end);
		Interval(float start, float end, bool start_bracket, bool end_bracket);
		virtual ~Interval();
		float getStart();
		float getEnd();
		bool getStartBracket();
		bool getEndBracket();
		string to_string();
	protected:
		float m_start;
		float m_end;
		// type of the brackets: : true = [, false = ]
		bool m_start_bracket;
		bool m_end_bracket;
};

#endif // INTERVAL_H

