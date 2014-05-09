/*!
 * \file predicate.h
 * \brief This class represent a PDDL predicate
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Predicate {
public:
	
	/*!
	 * \brief Constructor
	 */
	Predicate();
	
	/*!
	 * \brief Constructor
	 * \param name - name of the predicate
	 */
	Predicate(string name);
	
	/*!
	 * \brief Constructor
	 * \param name - name of the predicate
	 *        types_list - either type of each parameter
	 */
	Predicate(string name, vector< vector<Type*> > types_list);
	
	/*!
	 * \brief Destructor - virtual
	 */
	virtual ~Predicate();
	
	/*!
	 * \brief Get the name of the predicates
	 * \return the string of its name
	 */
	string getName();
	
	/*!
	 * \brief Get the either types of each parameters
	 * \return a pointer to list of either types of each parameters m_types_list
	 */
	vector< vector<Type*> > * getTypesList();
	
	/*!
	 * \brief Return a string that represent the predicate
	 * \return a string "Predicate name - either type of each parameter"
	 */
	string to_string();
	
private:
	/*!< name of the predicate */
	string m_name;
	/*!< list of either types that represent the types of each oredered parameter of the predicate */
	vector< vector<Type*> > m_types_list;
};

#endif // PREDICATE_H

