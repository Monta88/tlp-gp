/*!
 * \file interval.h
 * \brief This class represent an interval of which bounds are floats
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef INTERVAL_H
#define INTERVAL_H

#include <string>

using namespace std;

// note that if the fluent is set "at a" then start=a and end=a
class Interval {
public:
	
	/*!
	 * \brief Constructor (will set m_start_bracket to true, m_end_bracket to false, start to -1.0 and end to -1.0 to form the interval [-1.0; -1.0])
	 */
	Interval();
	
	/*!
	 * \brief Constructor (will set m_start_bracket to true and m_end_bracket to false to form the interval [start; end])
	 * \param start - time of starting
	 *        end - time of ending
	 */
	Interval(float start, float end);
	
	/*!
	 * \brief Constructor
	 * \param start - time of starting
	 *        end - time of ending
	 *        start_bracket - type of the opening bracket : true = [, false = ]
	 *        end_bracket - type of the closing bracket : true = [, false = ]
	 */
	Interval(float start, float end, bool start_bracket, bool end_bracket);
	
	/*!
	 * \brief Destructor - virtual
	 */
	virtual ~Interval();
	
	/*!
	 * \brief Get the starting time
	 * \return the float of starting bound
	 */
	float getStart();
	
	/*!
	 * \brief Get the ending time
	 * \return the float of ending bound
	 */
	float getEnd();
	
	/*!
	 * \brief Get the type of the opening bracket
	 * \return the type of the opening bracket : true = [, false = ]
	 */
	bool getStartBracket();
	
	/*!
	 * \brief Get the type of the closing bracket
	 * \return the type of the closing bracket : true = [, false = ]
	 */
	bool getEndBracket();
	
	/*!
	 * \brief Return a string that represent the interval
	 * \return a string "([ - ]) start; end ([ - ])"
	 */
	string to_string();
	
protected:
	/*!< time of the starting bound */
	float m_start;
	/*!< time of the ending bound */
	float m_end;
	/*!< type of the opening bracket : true = [, false = ] */
	bool m_start_bracket;
	/*!< type of the closing bracket : true = [, false = ] */
	bool m_end_bracket;
};

#endif // INTERVAL_H

