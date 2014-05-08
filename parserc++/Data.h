/*!
 * \file Data.h
 * \brief This class contains the data used by the parser generated with bisonc++ and flexc++
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#ifndef DATA_H_
#define DATA_H_

#include <string>
#include <vector>
#include "src/action.h"
#include "src/attribute.h"
#include "src/predicate.h"
#include "src/constant.h"
#include "src/domain.h"
#include "src/durative_action.h"
#include "src/fluent.h"
#include "src/function.h"
#include "src/interval.h"
#include "src/member.h"
#include "src/object.h"
#include "src/planningData.h"
#include "src/problem.h"
#include "src/type.h"
#include "src/typedList.h"
#include "src/variable.h"

using namespace std;

class Data {
public:
	/*!
	 * \brief Constructor
	 */
	Data();
	
	/*!
	 * \brief Destructor
	 */
	virtual ~Data();
	
	/*!
	 * \brief Set the name of the domain
	 * \param name - name of the domain
	 */
	void addDomain(string * name);
	
	/*!
	 * \brief Create a pointer to the domain
	 * \return a pointer to the domain
	 */
	Domain * getDomain();
	
	/*!
	 * \brief Create a pointer to the problem
	 * \return a pointer to the problem
	 */
	Problem * getProblem();
	
	/*!
	 * \brief Verify if name is the name of the domain previously parsed
	 * \param name - name of the domain of the problem
	 */
	bool isDomain(string * name);
	
	/*!
	 * \brief Add the requirement req in m_requirement once read in :requirements
	 * \param req - Token of the requirement
	 * \return false if the requirement already exists, else true
	 */
	bool addRequirement(int req);
	
	/*!
	 * \brief Verify if the requirement req has been announced in :requirements, and stored in m_requirements
	 * \param req - Token of the requirement
	 */
	bool isRequirement(int req);
	
	/*!
	 * \brief Add the types in m_types once read in :types
	 *        and add the name of this type in m_type_list
	 * \param typedList_list - list of types : (typesList, parentTypesList)
	 * \return false something went wrong
	 */
	bool addTypes(std::vector<TypedList*> * typedList_list);
	
	/*!
	 * \brief Verify if the type type has been defined, in m_type_list
	 * \param type - name of the type
	 */
	bool isType(string type);
	
	/*!
	 * \brief Get the pointer to a type contained in m_types
	 * \param name - name of the type
	 * \return a pointer to the type which name is name, or NULL if it doesn't exist
	 */
	Type * getType(string name);
	
	/*!
	 * \brief Add the constants in m_constants once read in :constants
	 *        and add the name of this constant in m_constant_list
	 * \param typedList_list - list of constants : (constantsList, typesList)
	 * \return false something went wrong
	 */
	bool addConstants(vector<TypedList*> * typedList_list);
	
	/*!
	 * \brief Verify if the constant constant has been defined, in m_constant_list
	 * \param constant - name of the constant
	 */
	bool isConstant(string constant);
	
	/*!
	 * \brief Get the pointer to a constant contained in m_constants
	 * \param constant - name of the constant
	 * \return a pointer to the constant which name is constant, or NULL if it doesn't exist
	 */
	Constant * getConstant(string constant);
	
	/*!
	 * \brief Add the predicates in m_predicates once read in :predicates
	 * \param name - the name of the predicate
	 *        typedList_list - list of parameters : (variablesList, typesList)
	 * \return false something went wrong
	 */
	bool addPredicate(string * name, vector<TypedList*> * typedList_list);
	
	/*!
	 * \brief Verify if the predicate have been defined in m_predicates
	 * \param name - the name of the predicate
	 *        typedList_list - list of parameters : (variablesList, typesList)
	 */
	bool isPredicate(string * name, vector<TypedList*> * typedList_list);
	
	/*!
	 * \brief Verify if the predicate have been defined in m_predicates
	 * \param name - the name of the predicate
	 *        types - list of parameters' types
	 */
	bool isPredicate(string * name, vector< vector<Type*> > types);
	
	/*!
	 * \brief Get the pointer to a predicate contained in m_predicates
	 * \param name - the name of the predicate
	 *        types - list of parameters' types
	 * \return a pointer to the predicate with the right name and parameters, or NULL if it doesn't exist
	 */
	Predicate * getPredicate(string * name,vector< vector<Type*> > types);
	
	/*!
	 * \brief Add the functions in m_functions once read in :functions
	 * \param function_skeleton_list - list of the functions' pairs of name and parameters
	 *        return_type - the type of the return (list for either representation)
	 * \return false something went wrong
	 */
	bool addFunctions(vector< pair< string*, vector<TypedList*>* >* > * function_skeleton_list, vector<string> * return_type);
	
	/*!
	 * \brief Add one function in m_functions
	 * \param name - name of the function
	 *        return_type - the type of the return
	 *        typedList_list - list of parameters of the function : (variablesList, typesList)
	 * \return false something went wrong
	 */
	bool addFunction(string * name, vector<Type*> return_type, vector<TypedList*> * typedList_list);
	
	/*!
	 * \brief Verify if the function have been defined in m_functions
	 * \param name - the name of the function
	 *        typedList_list - list of parameters : (variablesList, typesList)
	 */
	bool isFunction(string * name, vector<TypedList*> * typedList_list);
	
	/*!
	 * \brief Set the name of the problem
	 * \param name - name of the problem
	 */
	void addProblem(string * name);
	
	/*!
	 * \brief Add the objects in m_objects once read in :objects
	 *        and add the name of this object in m_object_list
	 * \param typedList_list - list of objects : (objectsList, typesList)
	 * \return false something went wrong
	 */
	bool addObjects(vector<TypedList*> * typedList_list);
	
	/*!
	 * \brief Verify if the object object has been defined, in m_object_list
	 * \param object - name of the object
	 */
	bool isObject(string object);
	
	/*!
	 * \brief Get the pointer to a object contained in m_objects
	 * \param object - name of the object
	 * \return a pointer to the object which name is object, or NULL if it doesn't exist
	 */
	Object * getObject(string object);
	
	/*!
	 * \brief Add an init fluent in m_inits once read in :init
	 * \param literal - fluent : pair(pair(parameters, name), positive)
	 *        at - time when this init is effctive
	 * \return false something went wrong, such as a fluent of which the predicate hasn't been defined
	 */
	bool addInit(pair< pair< vector< string > *, string *> *, bool > * literal, float at);
	
	/*!
	 * \brief Get a pointer to the list m_inits
	 * \return a pointer to m_inits
	 */
	vector<pair<Fluent*, Attribute> > * getInits();
	
	/*!
	 * \brief Add the goal fluents in m_goals once read in :goal
	 * \param pre_GD - list of fluents ()
	 * \return false something went wrong, such as a fluent of which the predicate hasn't been defined
	 */
	bool addGoals(vector< vector< pair< pair< vector< string > *, string *> * , int >* > * > * pre_GD);
	
	/*!
	 * \brief Get a pointer to the list m_goals
	 * \return a pointer to m_goals
	 */
	vector<pair<Fluent*, Attribute> > * getGoals();
	
	/*!
	 * \brief Verify if the durative-action name has been defined, in m_actions
	 * \param name - name of the durative-action
	 */
	bool isAction(string const * name);
	
	/*!
	 * \brief Get a pointer to the list m_actions
	 * \return a pointer to the list m_actions
	 */
	vector<DurativeAction*> * getActions();
	
	/*!
	 * \brief Display the data (domain variables, problem variables, and errors) in stdout
	 */
	void display();
	
	/*!
	 * \brief Add a warning in the list m_errors
	 * \param msg - message of the error
	 */
	void lexical_error(string msg);
	
	/*!
	 * \brief Stop the execution and print the error message on stderr
	 * \param msg - message of the error
	 */
	void fatal_error(string msg);
	
	/*!
	 * \brief Add adurative-action in m_actions once read in :durative-action
	 * \param name - name of the durative-action
	 *        typedList_list - parameters (variables) of the durative-action : (variablesList, typesList)
	 *        timed_GD - preconditions of the durative-action : list(pair(pair(fluentParameters, fluentName), precType))
	 *        cond_effect - effects of the durative-action : list(pair(pair(fluentParameters, fluentName), effectType))
	 * \return false something went wrong, such as a predicate that hasn't been defined
	 */
	bool addDurativeAction(string * name, vector<TypedList*> * typedList_list, float number, vector< pair< pair< vector< string > *, string *> *, int >* > * timed_GD, vector< pair< pair< vector< string > *, string *> * , int >* > * cond_effect);
	
	/*!
	 * \brief Add an initiated function in m_initiated_functions once read in :init
	 * \param atomic_formula - name and parameters : pair(parameters, name)
	 *        number - the return of the initiated function
	 * \return false something went wrong, such as a function or an object or a constant that hasn't been defined
	 */
	bool addInitiatedFunction(pair< vector< string > *, string *> * atomic_formula, float number);
	
	/*!
	 * \brief Verify if the function have been defined in m_functions
	 * \param name - the name of the function
	 *        types - list of parameters' types
	 */
	bool isFunction(string * name, vector< vector<Type*> > types);
	
	/*!
	 * \brief Get the pointer to a function contained in m_functions
	 * \param name - the name of the function
	 *        types - list of parameters' types
	 * \return a pointer to the function with the right name and parameters, or NULL if it doesn't exist
	 */
	Function * getFunction(string * name, vector< vector<Type*> > types);
	
	/*!
	 * \brief Get the return of an initiated function
	 * \param name - the name of the function
	 *        list_term - list of parameters
	 * \return the return of the function, and -1.0 if the function hasn't been found in m_initiated_functions
	 */
	float getFunctionReturn(string * name, vector<string> * list_term);
	
private:

	/////////////////////////////////
	////     local variables     ////
	/////////////////////////////////
	
	/*!< name of the domain */
	string m_domain;
	/*!< list of the PDDL requirements of the domain that correspond to the Parser's tokens (cf. the Token__ enum in Parserbase.h) */
	vector<int> m_requirements;
	/*!< list of the names of the PDDL types */
	vector<string> m_type_list;
	/*!< list of the names of the PDDL durative-actions */
	vector<string> m_list_name_action;
	/*!< list of the names of the PDDL constants */
	vector<string> m_constant_list;
	/*!< name of the problem */
	string m_problem;
	/*!< list of the names of the PDDL objects*/
	vector<string> m_object_list;
	/*!< list of the errors that happened */
	vector<string> m_errors;

	/////////////////////////////////
	////     domain variables    ////
	/////////////////////////////////
	
	/*!< list of the PDDL types */
	vector<Type*> * m_types;
	/*!< list of the PDDL constants */
	vector<Constant*> * m_constants;
	/*!< list of the PDDL predicates */
	vector<Predicate*> * m_predicates;
	/*!< list of the PDDL functions */
	vector<Function*> * m_functions;
	/*!< list of the PDDL durative-actions */	
	vector<DurativeAction*> * m_actions;	
	
	/////////////////////////////////
	////    problem variables    ////
	/////////////////////////////////
	
	/*!< list of the PDDL objects */
	vector<Object*> * m_objects;
	/*!< list of the PDDL functions associated with their return value defined in the :inits */
	vector< pair< pair<Function*, vector<Member*> >, float> > m_initiated_functions;
	/*!< list of the init fluents */
	vector<pair<Fluent*, Attribute> > * m_inits;
	/*!< list of the goal fluents */
	vector<pair<Fluent*, Attribute> > * m_goals;
};

#endif /* DATA_H_ */
