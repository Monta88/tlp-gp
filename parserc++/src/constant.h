/*!
 * \file constant.h
 * \brief This class represent a PDDL constant, inherit from Member
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef CONSTANT_H
#define CONSTANT_H
  
#include "member.h"


class Constant : public Member {
public:
	
	/*!
	 * \brief Constructor
	 */
	Constant();
	
	/*!
	 * \brief Constructor
	 * \param name - name of the constant
	 *        types - either type of the constant
	 */
	Constant(string name, vector<Type*> types);
	
	/*!
	 * \brief Add an either type to the existing either type of the constant (doesn't duplicate)
	 * \param types - either type to add to the constant
	 */
	void addTypes(vector<Type*> types);
	
	/*!
	 * \brief Return a string that represent the constant
	 * \return a string "Constant name - either type"
	 */
	string to_string();
	
	/*!
	 * \brief Return the name of the class
	 * \return "Constant"
	 */
	string getClass(); 
};

#endif // CONSTANT_H

