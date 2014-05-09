/*!
 * \file typedList.h
 * \brief This class represent a parser's typedList, which is a pair of a list of name and an either type
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef TYPEDLIST_H
#define TYPEDLIST_H
 
#include <string>
#include <vector>
#include "type.h"

using namespace std;


class TypedList {
public:
	
	/*!
	 * \brief Constructor
	 */
	TypedList();
	
	/*!
	 * \brief Constructor
	 * \param types - either type of the typedList
	 *        list - list of names of the typedList
	 */
	TypedList(vector<string> * types, vector<string> * list);
	
	/*!
	 * \brief Destructor
	 */
	~TypedList();
	
	/*!
	 * \brief Get the either type of the typedList
	 * \return a pointer to its either type m_types
	 */
	vector<string> * getTypes();
	
	/*!
	 * \brief Get the list of names of the typedList
	 * \return a pointer to its list of names m_list
	 */
	vector<string> * getList();
	
	/*!
	 * \brief Return a string that represent the typedList
	 * \return a string "TypedList : either type - list"
	 */
	string to_string();
	
private:
	/*!< either type associated with each member of which name is in m_list */
	vector<string> m_types;
	/*!< list of names of members */
	vector<string> m_list;
};

#endif // TYPEDLIST_H

