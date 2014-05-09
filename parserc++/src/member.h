/*!
 * \file member.h
 * \brief This class represent a PDDL constant, object or variable
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Member {
public:
	
	/*!
	 * \brief Constructor
	 */
	Member();
	
	/*!
	 * \brief Constructor
	 * \param name - name of the member
	 *        types - either type of the member
	 */
	Member(string name, vector<Type*> types);
	
	
	/*!
	 * \brief Destructor - virtual
	 */
	virtual ~Member();
	
	/*!
	 * \brief Change the name of the member
	 * \param name - new name of the member
	 */
	void changeName(string name);
	
	/*!
	 * \brief Get the name of the member
	 * \return the string of its name
	 */
	string getName();
	
	/*!
	 * \brief Get the either type of the member
	 * \return a pointer to its either type
	 */
	vector<Type*> * getTypes();
	
	/*!
	 * \brief Return a string that represent the member - virtual
	 * \return a string "Member name - either type"
	 */
	virtual string to_string();
	
	/*!
	 * \brief Return the name of the class - virtual
	 * \return "Member"
	 */
	virtual string getClass();
	
protected:
	/*!< name of the member */
	string m_name;
	/*!< either type of the member */
	vector<Type*> m_types;
};

#endif // MEMBER_H

