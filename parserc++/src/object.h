/*!
 * \file object.h
 * \brief This class represent a PDDL object, inherit from Member
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef OBJECT_H
#define OBJECT_H
 
#include "member.h"


class Object : public Member {
public:
	
	/*!
	 * \brief Constructor
	 */
	Object();
	
	/*!
	 * \brief Constructor
	 * \param name - name of the object
	 *        types - either type of the object
	 */
	Object(string name, vector<Type*> types);
	
	/*!
	 * \brief Add an either type to the existing either type of the object (doesn't duplicate)
	 * \param types - either type to add to the object
	 */
	void addTypes(vector<Type*> types);
	
	/*!
	 * \brief Return a string that represent the object
	 * \return a string "Object name - either type"
	 */
	string to_string();
	
	/*!
	 * \brief Return the name of the class
	 * \return "Object"
	 */
	string getClass(); 
};

#endif // OBJECT_H

