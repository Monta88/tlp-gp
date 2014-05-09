/*!
 * \file function.h
 * \brief This class represent a PDDL function
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#pragma once


#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include "type.h"

using namespace std;


class Function {
public:
	
	/*!
	 * \brief Constructor
	 */
	Function();
	
	/*!
	 * \brief Constructor
	 * \param name - name of the function
	 *        return_type - either type of the return of the function
	 *        types_list - either type of each parameter
	 */
	Function(string name, vector<Type*> return_type, vector< vector<Type*> > types_list);
	
	/*!
	 * \brief Destructor - virtual
	 */
	virtual ~Function();
	
	/*!
	 * \brief Get the name of the function
	 * \return the string of its name
	 */
	string getName();
	
	/*!
	 * \brief Get the either types of the return of the function
	 * \return a pointer to the either type m_return_type
	 */
	vector<Type*> * getReturnType();
	
	/*!
	 * \brief Get the either types of each parameters
	 * \return a pointer to list of either types of each parameters m_types_list
	 */
	vector< vector<Type*> > * getTypesList();
	
	/*!
	 * \brief Return a string that represent the function
	 * \return a string "Function name - either type of each parameter - either type of the return"
	 */
	string to_string();
	
private:
	/*!< name of the function */
	string m_name;
	/*!< either type of the return of the function */
	vector<Type*> m_return_type;
	/*!< list of either types that represent the types of each oredered parameter of the function */
	vector< vector<Type*> > m_types_list;
};

#endif // FUNCTION_H

