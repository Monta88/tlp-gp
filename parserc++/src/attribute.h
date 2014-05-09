/*!
 * \file attribute.h
 * \brief This class represent a PDDL Temporally Extended attribute (invented by Frederic Maris), which are the main feature of the interval of preconditions and effects of PDDL durative-actions
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>
#include "interval.h"

class Attribute {
public:
	
	/*!
	 * \brief Constructor
	 */
	Attribute();
	
	/*!
	 * \brief Destructor - virtual
	 */
	virtual ~Attribute();
	
	/*!
	 * \brief Get the duration of the list of interval that are supported
	 * \return the duration of the list of interval that are supported
	 */
	float getTime ();
	
	/*!
	 * \brief Add an interval to the list of interval that are forbidden
	 * \param inter - the new interval to add in m_forbidden
	 */
	void addForbidden(Interval inter);
	
	/*!
	 * \brief Add an interval to the list of interval that are not forbidden
	 * \param inter - the new interval to add in m_not_forbidden
	 */
	void addNotForbidden(Interval inter);
	
	/*!
	 * \brief Add an interval to the list of interval that are supported
	 * \param inter - the new interval to add in m_supported
	 */
	void addSupported(Interval inter);
	
	/*!
	 * \brief Add an interval to the list of interval that are not supported
	 * \param inter - the new interval to add in m_not_supported
	 */
	void addNotSupported(Interval inter);
	
	/*!
	 * \brief Get the list of interval that are forbidden
	 * \return a pointer to m_forbidden
	 */
	vector<Interval> * getForbidden();
	
	/*!
	 * \brief Get the list of interval that are supported
	 * \return a pointer to m_supported
	 */
	vector<Interval> * getSupported();
	
	/*!
	 * \brief Get the list of interval that are not forbidden
	 * \return a pointer to m_not_forbidden
	 */
	vector<Interval> * getNotForbidden();
	
	/*!
	 * \brief Get the list of interval that are not supported
	 * \return a pointer to m_not_supported
	 */
	vector<Interval> * getNotSupported();
	
	/*!
	 * \brief Return a string that represent the attribute
	 * \return a string "[forbidden on m_forbidden] [supported on m_supported] [not forbidden on m_not_forbidden] [not supported on m_not_supported]"
	 */
	string to_string();
	
private:
	/*!< list of interval of which characteristic is forbidden */
	vector<Interval> m_forbidden;
	/*!< list of interval of which characteristic is supported */
	vector<Interval> m_supported;
	/*!< list of interval of which characteristic is not forbidden */
	vector<Interval> m_not_forbidden;
	/*!< list of interval of which characteristic is not supported */
	vector<Interval> m_not_supported;
};

#endif // ATTRIBUTE_H

		
