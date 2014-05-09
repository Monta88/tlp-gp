/*!
 * \file type.h
 * \brief This class represent a PDDL type
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <vector>

using namespace std;


class Type {
public:
	
	/*!
	 * \brief Constructor
	 */
	Type();
	
	/*!
	 * \brief Constructor
	 * \param name - name of the type
	 *        parents - either type of the parent
	 */
	Type(string name, vector<Type*> parents);
	
	/*!
	 * \brief Constructor, the parent type is set to an empty vector
	 * \param name - name of the object
	 */
	Type(string name);
	
	/*!
	 * \brief Destructor - virtual
	 */
	virtual ~Type();
	
	/*!
	 * \brief Add an either type to the existing either type of the parent type (doesn't duplicate)
	 * \param types - either type to add to the object
	 */
	void addParents(vector<Type*> parents);
	
	/*!
	 * \brief Get the name of the type
	 * \return the string of the name
	 */
	string getName();
	
	/*!
	 * \brief Get the parent of the type
	 * \return a pointer to the either type of the parent
	 */
	vector<Type*> * getParents();
	
	/*!
	 * \brief Check recursively if a type is one the type's parent, in m_parents
	 * \param name - name of a the type to check in m_parents
	 * \return true is name is one of parent's type
	 */
	bool isOneOfParents(string name);
	
	/*!
	 * \brief Return a string that represent the type
	 * \return a string "Type name - Parent(s) either type"
	 */
	string to_string();
	
private:
	/*!< name of the type */
	string m_name;
	/*!< either type of the parent */
	vector<Type*> m_parents;
};

#endif // TYPE_H

