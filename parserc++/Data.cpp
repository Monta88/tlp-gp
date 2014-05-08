/*!
 * \file Data.cpp
 * \brief This class contains the data used by the parser generated with bisonc++ and flexc++
 * \author Alan BENIER, Martin LAGLEIZE, Nathan PRAT
 * \version 1.0
 * \date May 07th 2014
 */

#include "Data.h"

#include <algorithm> // to use find()
#include <iostream>

Data::Data() {
	// init
	
	m_requirements = vector<int> ();
	m_type_list = vector<string> ();
	m_constant_list = vector<string> ();
	m_types = new vector<Type*> ();
	m_constants = new vector<Constant*> ();
	m_predicates = new vector<Predicate*> ();
	m_functions = new vector<Function*> ();
	m_actions = new vector<DurativeAction*> ();
	
	m_object_list = vector<string> ();
	m_initiated_functions = vector< pair< pair<Function*, vector<Member*> >, float> > ();
	m_objects = new vector<Object*> ();
	m_inits = new vector<pair<Fluent*, Attribute> > ();
	m_goals = new vector<pair<Fluent*, Attribute> > ();
	
	// this type always exists and shouldn't be defined by the user in pddl file
	m_types->push_back(new Type("object"));
	m_type_list.push_back("object");
	m_types->push_back(new Type("objects"));
	m_type_list.push_back("objects");
	m_types->push_back(new Type("Object"));
	m_type_list.push_back("Object");
	m_types->push_back(new Type("Objects"));
	m_type_list.push_back("Objects");
	// for functions
	m_types->push_back(new Type("number"));
	m_type_list.push_back("number");
	
	m_errors = vector<string> ();
}

Data::~Data() {}

void Data::addDomain(string * name) {
	m_domain = *name;
}

Domain * Data::getDomain() {
	// creation of the new domain
	Domain * domain = new Domain(m_domain);
	domain->addTypes(m_types);
	domain->addConstants(m_constants);
	domain->addPredicates(m_predicates);
	domain->addFunctions(m_functions);
	domain->addActions(m_actions);
	
	return domain;
}

Problem * Data::getProblem() {
	// creation of the new problem
	Problem * problem = new Problem(m_problem);
	problem->addObjects(m_objects);
	problem->addInits(m_inits);
	problem->addGoals(m_goals);
	
	return problem;
}

bool Data::isDomain(string * name) {
	return (m_domain == (*name));
}

bool Data::addRequirement(int req) {
	if (isRequirement(req)) // if the requirement already exists
		return false;
	// if the requirement didn't exist
	m_requirements.push_back(req);
	return true;
}

bool Data::isRequirement(int req) {
	return (find(m_requirements.begin(), m_requirements.end(), req) != m_requirements.end());
}

bool Data::addTypes(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists, that's why we use a reverse_iterator
	
	// scan of the list of lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		// parent of the current types
		vector<Type*> parents = vector<Type*> ();
		
		// scan of the parent types in order to construct the parent of the types
		for (vector<string>::reverse_iterator it_parent = (*it)->getTypes()->rbegin(); it_parent != (*it)->getTypes()->rend(); ++it_parent) {
			if (!isType(*it_parent)) { // if the parent type doesn't exist, then we create it
				m_type_list.push_back(*it_parent);
				m_types->push_back(new Type(*it_parent));
				lexical_error("The " + (*it_parent) + " type didn't exist and so it have been assistly created for you");
			}
			
			parents.push_back(getType(*it_parent));
		}
		
		// scan of the types that have parents as parent types
		for (vector<string>::reverse_iterator it_type = (*it)->getList()->rbegin(); it_type != (*it)->getList()->rend(); ++it_type) {
			if (isType(*it_type)) { // if the type already exists, we add it its new parents
				getType(*it_type)->addParents(parents);
				lexical_error("The " + (*it_type) + " type already existed so its parents have been updated");
			}
			else { // if the type didn't exist, we create it
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type, parents));
			}
		}
	}
	
	return true;
}

bool Data::isType(string type) {
	return (find(m_type_list.begin(), m_type_list.end(), type) != m_type_list.end());
}

Type * Data::getType(string name) {
	// we scan the types list to find the right one
	for (vector<Type*>::iterator it = m_types->begin(); it != m_types->end(); ++it) {
		if ((*it)->getName() == name) { // if it is the right one
			return (*it);
		}
	}
	
	// if the needed type hasn't been found
	fatal_error("You tried to get the type : \""+ name +"\" but it doesn't exist");
	
	return NULL;
}

bool Data::addConstants(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists, that's why we use a reverse_iterator
	
	// we scan the list of lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		// type of the current constants
		vector<Type*> types = vector<Type*> ();
		
		// we scan the types of the constants that will be added in order to construct the type of the constants
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) { // if the type didn't exist, then we create it
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type didn't exist and so it have been assistly created for you");
			}
			types.push_back(getType(*it_type));
		}
		
		// we scan the constants that will be added
		for (vector<string>::reverse_iterator it_constant = (*it)->getList()->rbegin(); it_constant != (*it)->getList()->rend(); ++it_constant) {
			if (isConstant(*it_constant)) { // if the constant existed, then we update its type
				getConstant(*it_constant)->addTypes(types);
				lexical_error("The " + (*it_constant) + " constant already existed so its type has been updated");
			}
			else { // if the constant didn't exist, then we create it
				m_constant_list.push_back(*it_constant);
				m_constants->push_back(new Constant(*it_constant, types));
			}
		}
	}
	
	return true;
}

bool Data::isConstant(string constant) {
	return (find(m_constant_list.begin(), m_constant_list.end(), constant) != m_constant_list.end());
}

Constant * Data::getConstant(string constant) {
	// we scan the list of constants to find the right one
	for (vector<Constant*>::iterator it = m_constants->begin(); it != m_constants->end(); ++it) {
		if ((*it)->getName() == constant) { // if it is the right one
			return *it;
		}
	}
	
	// if the needed constant hasn't been found
	fatal_error("Constant \""+ constant +"\" not found");
	
	return NULL;
}

bool Data::addPredicate(string * name, vector<TypedList*> * typedList_list) {
	// the parser is recursive so we must reverse in order to have the arguments of the predicate in the same order than in the file, that's why we use a reverse_iterator
	
	if (isPredicate(name, typedList_list)) { // if the predicate already exists with the same name and the same parameters' types
		lexical_error("The " + (*name) + " predicate already exists with the same name and types, so it won't be used");
		return false;
	}
	
	// the list of the parameters' types
	vector< vector<Type*> > types_list = vector< vector<Type*> > ();
	
	// we scan the list of list that corresponds to the parameters
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		// type of the current variables
		vector<Type*> types = vector<Type*> ();
		
		// we scan the type's list
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) { // if the type didn't exist, then we create it
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type didn't exist and so it have been assistly created for you");
			}
			types.push_back(getType(*it_type));
		}
		
		// for each variable of this type, we add the type to the parameters' types
		for (vector<string>::reverse_iterator it_variable = (*it)->getList()->rbegin(); it_variable != (*it)->getList()->rend(); ++it_variable) {
			types_list.push_back(types);
		}
	}
	
	// we create and add the new predicate
	m_predicates->push_back(new Predicate(*name, types_list));
	
	return true;
}

bool Data::isPredicate(string * name, vector<TypedList*> * typedList_list) {
	unsigned int nb_parameters = 0; // contains the number of parameters of the predicate that is tested
	
	// we count the number of parameters of the tested predicate
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		nb_parameters += (*it)->getList()->size();
	}
	
	// we will test all predicate in m_predicates
	for (vector<Predicate*>::iterator it_predicate = m_predicates->begin(); it_predicate != m_predicates->end(); ++it_predicate) {
		if ((*name) == (*it_predicate)->getName()) { // if they don't have the same name, they are different
			if (nb_parameters == 0) { // if both have no parameters, they are equal
				return true;
			}
			
			if (nb_parameters == (*it_predicate)->getTypesList()->size()) { // if they don't have the same number of parameters, they are different
				bool same = true; // flag
				unsigned int total = 0; // number of parameters of the current reference predicate
				
				// for each list
				for (unsigned int i=typedList_list->size()-1; (i >= 0) && (total < nb_parameters) && same; --i) { // remeber we scan the list in reverse ordering
					// for each variable, we will test if the types are corresponding
					for (unsigned int j=0; (j < typedList_list->at(i)->getList()->size()) && same; ++j) { // no ordering needed
						bool contains = false; // flag
						
						// for each type of the "either" type of the current variable
						for (unsigned int k=0; (k < typedList_list->at(i)->getTypes()->size()) && same && !contains; ++k) { // no ordering needed
							// for each type of the "either" type of the current predicate's corresponding parameter
							for (vector<Type*>::iterator it = (*it_predicate)->getTypesList()->at(total+j).begin(); (it != (*it_predicate)->getTypesList()->at(total+j).end()) && !contains; ++it) {
								if ((*it)->getName() == typedList_list->at(i)->getTypes()->at(k)) { // if one of the types are corresponding, then we say that they are the same types
									contains = true;
								}
							}
						}
						
						if (!contains) { // if they have one of they parameters that doesn't correspond, they are different
							same = false;
						}
					}
					total += typedList_list->at(i)->getList()->size();
				}
				
				if (same) { // if they have the same types, they are equal
					return true;
				}
			}
		}
	}
	
	// predicate not found
	return false;
}

bool Data::addFunctions(vector< pair< string*, vector<TypedList*>* >* > * function_skeleton_list, vector<string> * return_type) {
	// the parser is recursive so we must reverse in order to have the arguments of the predicate in the same order than in the file, that's why we use a reverse_iterator
	
	bool success = true; // return in order not to stop the execution of this function when an error happen
	vector<Type*> return_t = vector<Type*> (); // return type of the functions to add
	
	// we scan the return type of the functions
	for (vector<string>::reverse_iterator it_type = return_type->rbegin(); it_type != return_type->rend(); ++it_type) {
		if (!isType(*it_type)) { // if the type didn't exist then we create it
			m_type_list.push_back(*it_type);
			m_types->push_back(new Type(*it_type));
			lexical_error("The " + (*it_type) + " type didn't exist and so it have been assistly created for you");
		}
		return_t.push_back(getType(*it_type));
	}
	
	// for each function
	for (vector< pair< string*, vector<TypedList*>* >* >::reverse_iterator it = function_skeleton_list->rbegin(); it != function_skeleton_list->rend(); ++it) {
		if (!addFunction((*it)->first,  return_t, (*it)->second)) { // if something went wrong during the adding of the function
			success = false;
		}
	}
	
	return success;
}

bool Data::addFunction(string * name, vector<Type*> return_type, vector<TypedList*> * typedList_list) {
	// the parser is recursive so we must reverse in order to have the arguments of the function in the same order than in the file, that's why we use a reverse_iterator
	
	if (isFunction(name, typedList_list)) { // if the function already exists with the same name and parameter's types
		lexical_error("The " + (*name) + " function already exists with the same name and types, so it won't be used");
		return false;
	}
	
	vector< vector<Type*> > types_list = vector< vector<Type*> > (); // types of the parameters
	
	// for each list
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> (); // type of the current variables
		
		// for each type in the either
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) { // if didn't exist then we create it
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type didn't exist and so it have been assistly created for you");
			}
			types.push_back(getType(*it_type));
		}
		
		// for each variable, we add the current type as parameter
		for (vector<string>::reverse_iterator it_variable = (*it)->getList()->rbegin(); it_variable != (*it)->getList()->rend(); ++it_variable) {
			types_list.push_back(types);
		}
	}
	
	// we create and add the new function
	m_functions->push_back(new Function(*name, return_type, types_list));
	
	return true;
}

bool Data::isFunction(string * name, vector<TypedList*> * typedList_list) {
	unsigned int nb_parameters = 0; // contains the number of parameters of the function that is tested
	
	// we count the number of parameters of the tested function
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		nb_parameters += (*it)->getList()->size();
	}
	
	// we will test all function in m_functions
	for (vector<Function*>::iterator it_function = m_functions->begin(); it_function != m_functions->end(); ++it_function) {
		if ((*name) == (*it_function)->getName()) { // if they don't have the same name, they are different
			if (nb_parameters == 0) { // if both have no parameters, they are equal
				return true;
			}
			
			if (nb_parameters == (*it_function)->getTypesList()->size()) { // if they don't have the same number of parameters, they are different
				bool same = true; // flag
				unsigned int total = 0; // number of parameters of the current reference function
				
				// for each list
				for (unsigned int i=typedList_list->size()-1; (i >= 0) && (total < nb_parameters) && same; --i) { // remeber we scan the list in reverse ordering
					// for each variable, we will test if the types are corresponding
					for (unsigned int j=0; (j < typedList_list->at(i)->getList()->size()) && same; ++j) {// no ordering needed
						bool contains = false; // flag
						
						// for each type of the "either" type of the current variable
						for (unsigned int k=0; (k < typedList_list->at(i)->getTypes()->size()) && same && !contains; ++k) {// no ordering needed
							// for each type of the "either" type of the current predicate's corresponding parameter
							for (vector<Type*>::iterator it = (*it_function)->getTypesList()->at(total+j).begin(); (it != (*it_function)->getTypesList()->at(total+j).end()) && !contains; ++it) {
								if ((*it)->getName() == typedList_list->at(i)->getTypes()->at(k)) { // if one of the types are corresponding, then we say that they are the same types
									contains = true;
								}
							}
						}
						if (!contains) { // if they have one of they parameters that doesn't correspond, they are different
							same = false;
						}
					}
					total += typedList_list->at(i)->getList()->size();
				}
				
				if (same) { // if they have the same types, they are equal
					return true;
				}
			}
		}
	}
	
	// function not found
	return false;
}

void Data::addProblem(string * name) {
	m_problem = *name;
}

bool Data::addObjects(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists, that's why we use a reverse_iterator
	
	// we scan de list of lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> (); // the current either type
		
		// we contruct the current either type
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) { // if the type didn't exist, then we create it
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type didn't exist and so it have been assistly created for you");
			}
			types.push_back(getType(*it_type));
		}
		
		// for each object
		for (vector<string>::reverse_iterator it_object = (*it)->getList()->rbegin(); it_object != (*it)->getList()->rend(); ++it_object) {
			if (isObject(*it_object)) { // if the object existed, then we update its type
				getObject(*it_object)->addTypes(types);
				lexical_error("The " + (*it_object) + " object already existed so its type has been updated");
			}
			else { // if the object didn't exist, then we create it
				m_object_list.push_back(*it_object);
				m_objects->push_back(new Object(*it_object, types));
			}
		}
	}
	return true;
}



bool Data::isObject(string object) {
	return (find(m_object_list.begin(), m_object_list.end(), object) != m_object_list.end());
}

Object * Data::getObject(string object) {
	// we scan the list of objects to find the right one
	for (vector<Object*>::iterator it = m_objects->begin(); it != m_objects->end(); ++it) {
		if ((*it)->getName() == object) { // if it is the right one
			return *it;
		}
	}
	
	// if the needed object hasn't been found
	fatal_error("Object \""+ object +"\" not found");
	
	return NULL;
}

bool Data::addInit(pair< pair< vector< string > *, string *> *, bool > * literal, float at) {
	vector<vector<Type*> > type_list = vector<vector<Type*> > (); // types of the parameters of the predicate of the fluent (theese types are thoose of members_list)
	vector<Member *> members_list = vector<Member *> (); // list of parameters of the fluent
	Member * member; // current parameter
	Predicate * predicate; // predicate of the current fluent
	Attribute attribute; // attribute associated with the current fluent
	
	// for each parameter
	for (vector<string>::reverse_iterator it_member = literal->first->first->rbegin(); it_member != literal->first->first->rend(); ++it_member) {
		if (isConstant(*it_member)) { // if it is a constant
			member = getConstant(*it_member);
		}
		else {
			if (isObject(*it_member)) { // or an object
				member = getObject(*it_member);
			}
			else { // if it isn't a constant nor an object
				fatal_error((*it_member) + " is nor a Constant nor an Object in the init of " + *literal->first->second);
				return false;
			}
		}
		members_list.push_back(member);
		type_list.push_back(*(member->getTypes()));
	}
	
	if (isPredicate(literal->first->second, type_list)) { // if the associated predicate exists, then we get it
		predicate = getPredicate(literal->first->second, type_list);
	}
	else { // if the associated predicate doesn't exists, error
		fatal_error("Predicate \""+ *(literal->first->second) +"\" not found in the inits");
		return false;
	}
	
	// we associate the fluent with its attribute
	attribute = Attribute();
	if (literal->second) { // if the fluent is positive
		attribute.addSupported(Interval(at, at));
	}
	else { // if the fluent is negative
		attribute.addNotSupported(Interval(at, at));
	}
	
	// we create and add the new fluent
	m_inits->push_back(make_pair(new Fluent(predicate, members_list), attribute));
	
	return true;
}

bool Data::addGoals(vector< vector< pair< pair< vector< string > *, string *> *, int >* > * > * pre_GD) { 
	vector<Member *> members_list = vector<Member *> (); // list of parameters of the fluent
	Member * member; // current parameter
	vector< vector<Type*> > type_list; // types of the parameters of the predicate of the fluent (theese types are thoose of members_list)
	Predicate * predicate; // predicate of the current fluent
	Attribute att; // attribute associated with the current fluent
	
	// for each goal list (at first, we used a list of lists of goal to manage the recursive AND in the parser, but now we take only the last AND in the goal, a modification in the parser's structure pre_GD must be done, to modify the one in Data)
	for (vector< vector< pair< pair< vector< string > *, string *> *, int >* > * >::reverse_iterator it_main = pre_GD->rbegin(); it_main != pre_GD->rend(); it_main++) {
		// for each goal
		for(vector< pair< pair<vector<string> *,string *> *, int> * >::reverse_iterator it = (*it_main)->rbegin(); it != (*it_main)->rend(); ++it) {			
			members_list = vector<Member *> ();
			type_list = vector< vector<Type*> > ();
			
			// for each parameter
			for (vector<string>::reverse_iterator it_member =(*it)->first->first->rbegin(); it_member != (*it)->first->first->rend(); ++it_member) {
				if (isConstant(*it_member)) { // if it is a constant
					member = getConstant(*it_member);
				}
				else {
					if (isObject(*it_member)) { // or an object
						member = getObject(*it_member);
					}
					else { // if it isn't a constant nor an object
						lexical_error((*it_member) + " is nor a Constant nor an Object in the goal of " + *(*it)->first->second);
						return false;
					}
				}
				members_list.push_back(member);
				type_list.push_back(*(member->getTypes()));
			}
			
			if (isPredicate((*it)->first->second, type_list)) { // if the associated predicate exists, then we get it
				predicate = getPredicate((*it)->first->second, type_list);
			}
			else { // if the associated predicate doesn't exists, error
				fatal_error("Predicate \""+ *(*it)->first->second +"\" not found in the goals");
				return false;
			}
			
			// this part is useless, but it allows to have the same structure than the inits, and define negative goals even they are incompatible with TLP-GP
			att = Attribute();
			if  (!((*it)->second & 0b1)){
				att.addSupported(Interval(0., 0.));
			} else {
				att.addNotSupported(Interval(0., 0.));
			}
			
			// we create and add the new fluent
			m_goals->push_back(make_pair(new Fluent(predicate, members_list), att));
		}
	}
	
	return true;
}

vector<pair<Fluent*, Attribute> > * Data::getGoals() {
	return m_goals;
}

void Data::display() {
	// name of the domain
	cout << "Domain : " << m_domain << endl;
	
	// types and their parents
	if (m_types->size() != 0) {
		cout << "Types : " << endl;
		for(vector<Type*>::iterator it = m_types->begin(); it != m_types->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	// constants and their type
	if (m_constants->size() != 0) {
		cout << "Constants : " << endl;
		for(vector<Constant*>::iterator it = m_constants->begin(); it != m_constants->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	// predicates and their parameters' types
	if (m_predicates->size() != 0) {
		cout << "Predicates : " << endl;
		for(vector<Predicate*>::iterator it = m_predicates->begin(); it != m_predicates->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	// functions and their parameters' types
	if (m_functions->size() != 0) {
		cout << "Functions : " << endl;
		for(vector<Function*>::iterator it = m_functions->begin(); it != m_functions->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	// name of the problem
	cout << "Problem : " << m_problem << endl;
	
	// objects and their type
	if (m_objects->size() != 0) {
		cout << "Objects : " << endl;
		for(vector<Object*>::iterator it = m_objects->begin(); it != m_objects->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	// init fluents and their attribute
	if (m_inits->size() != 0) {
		cout << "Inits : " << endl;
		for(vector<pair<Fluent*, Attribute> >::iterator it = m_inits->begin(); it != m_inits->end(); ++it)
			cout << "\t" << (*it).first->to_string() << " <-> " << (*it).second.to_string() << endl;
	}
	
	// goal fluents and their attribute
	if (m_goals->size() != 0) {
		cout << "Goals : " << endl;
		for(vector<pair<Fluent*, Attribute> >::iterator it = m_goals->begin(); it != m_goals->end(); ++it)
			cout << "\t" << (*it).first->to_string() << " <-> " << (*it).second.to_string() << endl;
	}
	
	// lexical warnings
	if (m_errors.size() != 0) {
		cerr << "Warning : " << endl;
		for(vector<string>::iterator it = m_errors.begin(); it != m_errors.end(); ++it)
			cerr << "\t" << (*it) << endl;
	}
	
	// durative-actions and their parameters, preconditions and effects
	if (m_actions->size() != 0) {
		cout << "Actions :" << endl;
		for(vector<DurativeAction*>::iterator it = m_actions->begin(); it != m_actions->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
}

void Data::lexical_error(string msg) {
	if (find(m_errors.begin(), m_errors.end(), msg) == m_errors.end()) { // we don't duplicate error
		m_errors.push_back(msg);
	}
}

void Data::fatal_error(string msg) {
	cerr << "ERROR : " << msg << endl << "Aborting..." << endl;
	exit(-1);
}

bool Data::isAction(string const * name){
	return (find(m_list_name_action.begin(), m_list_name_action.end(), *name) !=  m_list_name_action.end());
}

bool Data::isPredicate(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0; // contains the number of parameters of the predicate that is tested
	
	// we count the number of parameters of the tested predicate
	nb_parameters = types.size();
	
	// we will test all predicate in m_predicates
	for (vector<Predicate*>::iterator it_predicate = m_predicates->begin(); it_predicate != m_predicates->end(); ++it_predicate) {
		if ((*name) == (*it_predicate)->getName()) { // if they don't have the same name, they are different
			if (nb_parameters == 0) { // if both have no parameters, they are equal
				return true;
			}
			
			if (nb_parameters == (*it_predicate)->getTypesList()->size()) { // if they don't have the same number of parameters, they are different
				bool same = true; // flag
				
				// for each "either" type of the list types
				for (unsigned int i=0; (i < types.size()) && same; ++i) { // now we have the right ordering
					bool contains = false; // flag
					
					// for each type of the  current "either" type
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) { // no ordering needed
						// for each type of the "either" type of the current predicate's corresponding parameter
						for (vector<Type*>::iterator it = (*it_predicate)->getTypesList()->at(i).begin(); (it != (*it_predicate)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) { // if one of the types are corresponding, then we say that they are the same types
								contains = true;
							}
						}
					}
					if (!contains) { // if they have one of they parameters that doesn't correspond, they are different
						same = false;
					}
				}
				
				if (same) { // if they have the same types, they are equal
					return true;
				}
			}
		}
	}
	
	// predicate not found
	return false;
}

Predicate * Data::getPredicate(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0; // contains the number of parameters of the predicate that is tested
	
	// we count the number of parameters of the tested predicate
	nb_parameters = types.size();
	
	// we will test all predicate in m_predicates
	for (vector<Predicate*>::iterator it_predicate = m_predicates->begin(); it_predicate != m_predicates->end(); ++it_predicate) {
		if ((*name) == (*it_predicate)->getName()) { // if they don't have the same number of parameters, they are different
			if (nb_parameters == 0) { // if both have no parameters, they are equal
				return *it_predicate;
			}
			
			if (nb_parameters == (*it_predicate)->getTypesList()->size()) { // if they don't have the same number of parameters, they are different
				bool same = true; // flag
				
				// for each "either" type of the list types
				for (unsigned int i=0; (i < types.size()) && same; ++i) { // now we have the right ordering
					bool contains = false; // flag
					
					// for each type of the  current "either" type
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) { // no ordering needed
						// for each type of the "either" type of the current predicate's corresponding parameter
						for (vector<Type*>::iterator it = (*it_predicate)->getTypesList()->at(i).begin(); (it != (*it_predicate)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) { // if one of the types are corresponding, then we say that they are the same types
								contains = true;
							}
						}
					}
					if (!contains) { // if they have one of they parameters that doesn't correspond, they are different
						same = false;
					}
				}
				
				if (same) { // if they have the same types, they are equal
					return *it_predicate;
				}
			}
		}
	}
	
	// predicate not found
	fatal_error("You tried to get the predicate \""+ *name +"\" but couldn't find it");
	
	return NULL;
}

bool Data::addDurativeAction(string * name, vector<TypedList*> * typedList_list, float number, vector< pair< pair< vector< string > *, string *> *, int >* > * timed_GD, vector< pair< pair< vector< string > *, string *> *, int >* > * cond_effect) {
	DurativeAction * action; // action to add
	vector<Type *> types; // either type of the current parameter
	vector<Member *> variable_list =  vector<Member *> (); // list of the parameters of the action
	vector< vector<Type*> > type_list; // list of either types of the current fluent's parameters
	Fluent * fluent; // current fluent
	Attribute att; // attribute associated with the current fluent
	
	if (isAction(name)) { // if the action already exists
			lexical_error("The " + *name + " action already exists with the same name, so it won't be used");
			return false;
	}
	
	// we will add the missing attributes later
	action = new DurativeAction(*name);
	
	// action's parameters
	// for each vairiable type, we need to check if the type exists and add each variable with its type
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		types = vector<Type *> ();
		
		// for each type in the either type
		for (vector<string>::reverse_iterator it2 = (*it)->getTypes()->rbegin(); it2 != (*it)->getTypes()->rend(); ++it2) {
			 if (! isType(*it2)) { // if the type didn't exist then we create it
				m_type_list.push_back(*it2);
				m_types->push_back(new Type(*it2));
				lexical_error("The " + (*it2) + " type didn't exist and so it have been assistly created for you");
				return false;
			}
			types.push_back(getType(*it2));
		}
		
		// for each variable in the current typedList
		for (vector<string>::reverse_iterator it2 =(*it)->getList()->rbegin(); it2 != (*it)->getList()->rend();++it2) {					
			if (action->isVariable(*it2)) { // if the variable already is a parameter of this action (same name)
				lexical_error("In action " + action->getName() + " : the " + (*it2) + " variable already exist. The definition of this action is illed formed so it won't be used");
				return false;
			}	
			action->addParameters( Variable(*it2,types) );					
		}
	}
	
	// action's duration
	action->addDuration(number);
	
	// action's preconditions
	// for each fluent in the precondition list
	for(vector< pair< pair<vector<string> *,string *> * , int> * >::reverse_iterator it = timed_GD->rbegin(); it != timed_GD->rend(); ++it) {
		// each variable needs to be defined in parameters
		variable_list = vector<Member *>();
		type_list = vector< vector<Type*> >();
		
		// for each parameter of the fluent
		for (vector<string>::reverse_iterator it2 =(*it)->first->first->rbegin(); it2 != (*it)->first->first->rend(); ++it2){
			if( !(action->isVariable(*it2)) ) { // if it is not a parameter
				lexical_error("In action " + action->getName() + " : the variable " + (*it2) + " doesn't exist");
				return false;
			}
			type_list.push_back(*(action->getVariable(*it2)->getTypes()));
			variable_list.push_back(action->getVariable(*it2));	
		}
		
		if(!(isPredicate((*it)->first->second, type_list))) { // if the predicate doesn't exist
			lexical_error("In action " + action->getName() + " : the predicate "+ *(*it)->first->second +" doesn't exist. The definition of this action is illed formed so it won't be used");
			return false;
		}
		
		fluent = new Fluent (getPredicate((*it)->first->second, type_list),variable_list);
		
		att =  Attribute();
		switch (((*it)->second & 0b111110) >> 1) {
			case 0b00001:		// at start
				att.addSupported(Interval(0.,0.));
				break;
			case 0b00010:		// at end
				att.addSupported(Interval(number,number));
				break;
			case 0b00100:	 	// over all
				att.addSupported(Interval(0.,number));
				break;
			case 0b01000:		// supported all
				att.addSupported(Interval(0.,number));
				break;
			case 0b10000:		// forbidden all
				att.addForbidden(Interval(0.,number));
				break;
			default:
				fatal_error("Something went wrong while reading preconditions of the action " + action->getName() );
				return false;
		}
		
		if  (!((*it)->second & 0b1)) { // if fluent is positive
			action->addCondition(att,fluent);
		}
		else { // if fluent is negative
			action->addNotCondition(att,fluent);
		}
	}
	
	// action's effects
	// for each fluent in the effects
	for(vector< pair< pair<vector<string> *,string *> *, int> * >::reverse_iterator it = cond_effect->rbegin(); it != cond_effect->rend(); ++it) {
		// each variable needs to be defined in parameters
		variable_list = vector<Member *>();
		type_list = vector< vector<Type*> >();
		
		// for each parameter of the fluent
		for (vector<string>::reverse_iterator it2 =(*it)->first->first->rbegin(); it2 != (*it)->first->first->rend(); ++it2) {
			if( !(action->isVariable(*it2)) ) { // if the predicate doesn't exist
				lexical_error("In action " + action->getName() + " : the variable " + (*it2) + " doesn't exist. The definition of this action is illed formed so it won't be used");
				return false;
			}
			type_list.push_back(*(action->getVariable(*it2)->getTypes()));
			variable_list.push_back(action->getVariable(*it2));	
		}
		// predicate existence verification 
		if(!(isPredicate((*it)->first->second,type_list))) {
			lexical_error("In action " + action->getName() + " : the predicate "+ *(*it)->first->second + " doesn't exist. The definition of this action is illed formed so it won't be used");
			return false;
		}
		
		fluent = new Fluent (getPredicate((*it)->first->second, type_list),variable_list);
		
		att =  Attribute();
		switch (((*it)->second & 0b1111111110) >> 1) {
			case 0b000000001:		// at start
				att.addSupported(Interval(0.,0.));
				break;
			case 0b000000010:		// at end
				att.addSupported(Interval(number,number));
				break;
			case 0b000000100:		// over all
				att.addSupported(Interval(0.,number));
				break;
			case 0b000001000:		// supported all
				att.addSupported(Interval(0.,number));
				break;
			case 0b000010000:		// forbidden all
				att.addForbidden(Interval(0.,number));
				break;
			case 0b100000000:		// ->over all
				att.addSupported(Interval(number,number));
				att.addForbidden(Interval(0.,number));
				att.addNotForbidden(Interval(0.,number, false, false));
				break;
			default:
				fatal_error("Something went wrong while reading effects of the action " + action->getName() );
				return false;
		}
		
		if  (!((*it)->second & 0b1)) { // if the fluent is positive
			action->addEffect(att,fluent);
		}
		else { // if the fluent is negative
			action->addNotEffect(att,fluent);
		}
	}
	
	// we add the action to the list
	m_list_name_action.push_back(action->getName());
	m_actions->push_back(action);
	
	return true;
}

vector<pair<Fluent*, Attribute> > * Data::getInits(){
	return m_inits;
}

vector<DurativeAction*> * Data::getActions(){
	return m_actions;
}

bool Data::addInitiatedFunction(pair< vector< string > *, string *> * atomic_formula, float number) {
	vector<vector<Type*> > type_list = vector<vector<Type*> > (); // list of the either types of the parameters
	vector<Member *> members_list = vector<Member *> (); // list of the parameters
	Member * member; // current parameter
	Function * function; // current function
	
	// for each parameter
	for (vector<string>::reverse_iterator it_member = atomic_formula->first->rbegin(); it_member != atomic_formula->first->rend(); ++it_member) {
		if (isConstant(*it_member)) { // if the parameter is a constant
			member = getConstant(*it_member);
		}
		else {
			if (isObject(*it_member)) { // or an object
				member = getObject(*it_member);
			}
			else { // if the parameter is nor an object nor a constant
				lexical_error((*it_member) + " is nor a Constant nor an Object. The initiated function " + *atomic_formula->second + " won't be used");
				return false;
			}
		}
		members_list.push_back(member);
		type_list.push_back(*(member->getTypes()));
	}
	
	if (isFunction(atomic_formula->second, type_list)) { // if it's a function in m_functions
		function = getFunction(atomic_formula->second, type_list);
	}
	else { // if it doesn't correpond to a previously defined function
		fatal_error("Function \""+ *(atomic_formula->second) +"\" not found");
		return false;
	}
	
	// then we add the function, associated with its parameter and its return value
	m_initiated_functions.push_back(make_pair(make_pair(function, members_list), number));
	
	return true;
}

bool Data::isFunction(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0; // contains the number of parameters of the function that is tested
	
	// we count the number of parameters of the tested function
	nb_parameters = types.size();
	
	// we will test all function in m_functions
	for (vector<Function*>::iterator it_function = m_functions->begin(); it_function != m_functions->end(); ++it_function) {
		if ((*name) == (*it_function)->getName()) { // if they don't have the same number of parameters, they are different
			if (nb_parameters == 0) { // if both have no parameters, they are equal
				return true;
			}
			
			// for each parameter
			if (nb_parameters == (*it_function)->getTypesList()->size()) { // if they don't have the same number of parameters, they are different
				bool same = true; // flag
				
				// for each type of the  current "either" type
				for (unsigned int i=0; (i < types.size()) && same; ++i) { // now we have the right ordering
					bool contains = false; // flag
					
					// for each type of the  current "either" type
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) { // no ordering needed
						// for each type of the "either" type of the current predicate's corresponding parameter
						for (vector<Type*>::iterator it = (*it_function)->getTypesList()->at(i).begin(); (it != (*it_function)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) { // if one of the types are corresponding, then we say that they are the same types
								contains = true;
							}
						}
					}
					
					if (!contains) { // if they have one of they parameters that doesn't correspond, they are different
						same = false;
					}
				}
				
				if (same) { // if they have the same types, they are equal
					return true;
				}
			}
		}
	}
	
	// function not found
	return false;
}

Function * Data::getFunction(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0; // contains the number of parameters of the function that is tested
	
	// we count the number of parameters of the tested function
	nb_parameters = types.size();
	
	// we will test all function in m_functions
	for (vector<Function*>::iterator it_function = m_functions->begin(); it_function != m_functions->end(); ++it_function) {
		if ((*name) == (*it_function)->getName()) { // if they don't have the same number of parameters, they are different
			if (nb_parameters == 0) { // if both have no parameters, they are equal
				return (*it_function);
			}
			
			// for each "either" type of the list types
			if (nb_parameters == (*it_function)->getTypesList()->size()) { // if they don't have the same number of parameters, they are different
				bool same = true; // flag
				
				// for each parameter
				for (unsigned int i=0; (i < types.size()) && same; ++i) { // now we have the right ordering
					bool contains = false; // flag
					
					// for each type of the  current "either" type
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) { // no ordering needed
						// for each type of the "either" type of the current predicate's corresponding parameter
						for (vector<Type*>::iterator it = (*it_function)->getTypesList()->at(i).begin(); (it != (*it_function)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) { // if one of the types are corresponding, then we say that they are the same types
								contains = true;
							}
						}
					}
					
					if (!contains) { // if they have one of they parameters that doesn't correspond, they are different
						same = false;
					}
				}
				
				if (same) { // if they have the same types, they are equal
					return (*it_function);
				}
			}
		}
	}
	
	// function not found
	fatal_error("You tried to get the function \""+ *name +"\" but couldn't find it");
	
	return NULL;
}

float Data::getFunctionReturn(string * name, vector<string> * list_term) {
	unsigned int nb_parameters = 0; // contains the number of parameters of the function that is tested
	
	// we count the number of parameters of the tested function
	nb_parameters = list_term->size();
	
	// we will test all function in m_initiated_functions
	for (vector< pair< pair<Function*, vector<Member*> >, float> >::iterator it_initiated_function = m_initiated_functions.begin(); it_initiated_function != m_initiated_functions.end(); ++it_initiated_function) {
		if ((*name) == (*it_initiated_function).first.first->getName()) { // if they don't have the same number of parameters, they are different
			if (nb_parameters == 0) { // if both have no parameters, they are equal
				return (*it_initiated_function).second;
			}
			
			if (nb_parameters == (*it_initiated_function).first.second.size()) { // if they don't have the same number of parameters, they are different
				bool same = true; // flag
				
				// for each parameter
				for (unsigned int i=0; (i < list_term->size()) && same; ++i) {
					if (list_term->at(i) != (*it_initiated_function).first.second.at(i)->getName()) { // if they are the same
						same = false;
					}
				}
				
				if (same) { // if they have the same parameters
					return (*it_initiated_function).second;
				}
			}
		}
	}
	
	// initiated function not found
	fatal_error("You tried to get the return of \""+ *name +"\" function but couldn't find it");
	
	return -1.;
}


