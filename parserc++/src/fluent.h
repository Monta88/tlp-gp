/*!
 * \file fluent.h
 * \brief This class represent a fluent, which is a predicate of which parameters have been set
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef FLUENT_H
#define FLUENT_H

#include <string>
#include <vector>
#include "member.h"
#include "predicate.h"

using namespace std;


class Fluent {
public:
	
	/*!
	 * \brief Constructor
	 */
	Fluent();
	
	/*!
	 * \brief Constructor
	 * \param predicate - predicate to link (can be used if the predicate doesn't have any parameters)
	 */
	Fluent(Predicate * predicate);
	
	/*!
	 * \brief Constructor
	 * \param predicate - predicate to link
	 *        members_list - list of parameters
	 */
	Fluent(Predicate * predicate, vector<Member*> members_list);
	
	/*!
	 * \brief Destructor - virtual
	 */
	virtual ~Fluent();
	
	/*!
	 * \brief Get the predicate linked to the fluent
	 * \return the pointer to the predicate of this fluent
	 */
	Predicate * getPredicate();
	
	/*!
	 * \brief Get the parameters
	 * \return a pointer to the list of parameters which is a list of members
	 */
	vector<Member*> * getMembersList();
	
	/*!
	 * \brief Set the parameters
	 * \param member - list of parameters to set
	 */
	void addMember(vector<Member*> member);
	
	/*!
	 * \brief Add one parameter at the end of the list of parameters
	 * \param member - parameter to add at the end of the list
	 */
	void addMember(Member * member);
	
	/*!
	 * \brief Return a string that represent the fluent
	 * \return a string "Fluent name of the predicate - names of the parameter"s
	 */
	string to_string();
	
private:
	/*!< pointer to the predicate used by this fluent */
	Predicate * m_predicate;
	/*!< list of members parameters (must have the same types than those of m_predicate) */
	vector<Member*> m_members_list;
};

#endif // FLUENT_H

