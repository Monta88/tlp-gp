/*!
 * \file variable.h
 * \brief This class represent a PDDL variable, inherit from Member
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef VARIABLE_H
#define VARIABLE_H

#include "member.h"


class Variable : public Member {
public:
	
	/*!
	 * \brief Constructor
	 */
	Variable();
	
	/*!
	 * \brief Constructor
	 * \param name - name of the variable
	 *        types - either type of the variable
	 */
	Variable(string name, vector<Type*> types);
	
	/*!
	 * \brief Return a string that represent the variable
	 * \return a string " Variable name - either type"
	 */
	string to_string();
	
	/*!
	 * \brief Return the name of the class
	 * \return "Variable"
	 */
	string getClass();
};

#endif // VARIABLE_H

