#include "Data.h"

#include <algorithm>
#include <iostream>

Data::Data() {
	m_domain = new Domain();
	m_requirements = vector<int> ();
	m_type_list = vector<string> ();
	m_constant_list = vector<string> ();
	m_types = vector<Type*> ();
	m_constants = vector<Constant*> ();
	m_predicates = vector<Predicate*> ();
	m_functions = vector<Function*> ();
	
	m_problem = Problem();
	m_object_list = vector<string> ();
	m_objects = vector<Object*> ();
	
	// this type always exists and shouldn't be defined by the user in pddl file
	m_types.push_back(new Type("object"));
	m_type_list.push_back("object");
	m_types.push_back(new Type("objects"));
	m_type_list.push_back("objects");
	m_types.push_back(new Type("Object"));
	m_type_list.push_back("Object");
	m_types.push_back(new Type("Objects"));
	m_type_list.push_back("Objects");
	// for functions
	m_types.push_back(new Type("number"));
	m_type_list.push_back("number");
	
	m_errors = vector<string> ();
}

Data::~Data() {}

void Data::addDomain(string * name) {
	m_domain = new Domain(*name);
}

bool Data::isDomain(string * name) {
	return (m_domain->getName() == (*name));
}

bool Data::addRequirement(int req) {
	if (isRequirement(req)) {
		return false;
	}
	m_requirements.push_back(req);
	return true;
}

bool Data::isRequirement(int req) {
	return (find(m_requirements.begin(), m_requirements.end(), req) != m_requirements.end());
}

bool Data::addTypes(std::vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> parents = vector<Type*> ();
		for (vector<string>::reverse_iterator it_parent = (*it)->getTypes()->rbegin(); it_parent != (*it)->getTypes()->rend(); ++it_parent) {
			if (!isType(*it_parent)) {
				lexical_error("The " + (*it_parent) + " type don't exist");
				return false;
			}
			parents.push_back(getType(*it_parent));
		}
		for (vector<string>::reverse_iterator it_type = (*it)->getList()->rbegin(); it_type != (*it)->getList()->rend(); ++it_type) {
			if (isType(*it_type)) {
				lexical_error("The " + (*it_type) + " type already exists");
				return false;
			}
			m_type_list.push_back(*it_type);
			m_types.push_back(new Type(*it_type, parents));
		}
	}
	return true;
}

bool Data::isType(string type) {
	return (find(m_type_list.begin(), m_type_list.end(), type) != m_type_list.end());
}

Type * Data::getType(string name) {
	for (vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it) {
		if ((*it)->getName() == name) {
			return (*it);
		}
	}
	fatal_error("You try to get a Type* : \""+ name +"\" but can't find him");
	return NULL;
}

bool Data::addConstants(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> ();
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) {
				lexical_error("The " + (*it_type) + " type don't exist");
				return false;
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_constant = (*it)->getList()->rbegin(); it_constant != (*it)->getList()->rend(); ++it_constant) {
			if (isConstant(*it_constant)) {
				lexical_error("The " + (*it_constant) + " constant already exists");
				return false;
			}
			m_constant_list.push_back(*it_constant);
			m_constants.push_back(new Constant(*it_constant, types));
		}
	}
	return true;
}

bool Data::isConstant(string constant) {
	return (find(m_constant_list.begin(), m_constant_list.end(), constant) != m_constant_list.end());
}

bool Data::addPredicate(string * name, vector<TypedList*> * typedList_list) {
	// the parser is recursive so we must reverse in order to have the arguments of the predicate in the same order than in the file
	if (isPredicate(name, typedList_list)) {
		lexical_error("The " + (*name) + " predicate already exists with the same types");
		return false;
	}
	vector< vector<Type*> > types_list = vector< vector<Type*> > ();
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> ();
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) {
				lexical_error("The " + (*it_type) + " type don't exist");
				return false;
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_variable = (*it)->getList()->rbegin(); it_variable != (*it)->getList()->rend(); ++it_variable) {
			types_list.push_back(types);
		}
	}
	m_predicates.push_back(new Predicate(*name, types_list));
	return true;
}

bool Data::isPredicate(string * name, vector<TypedList*> * typedList_list) {
	unsigned int nb_parameters = 0;
	
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		nb_parameters += (*it)->getList()->size();
	}
	
	for (vector<Predicate*>::iterator it_predicate = m_predicates.begin(); it_predicate != m_predicates.end(); ++it_predicate) {
		if ((*name) == (*it_predicate)->getName()) {
			if (nb_parameters == 0) {
				return true;
			}
			
			if (nb_parameters == (*it_predicate)->getTypesList()->size()) {
				bool same = true;
				unsigned int total = 0;
				for (unsigned int i=typedList_list->size()-1; (i >= 0) && (total < nb_parameters) && same; --i) {
					for (unsigned int j=0; (j < typedList_list->at(i)->getList()->size()) && same; ++j) {
						bool contains = false;
						for (unsigned int k=0; (k < typedList_list->at(i)->getTypes()->size()) && same && !contains; ++k) {
							for (vector<Type*>::iterator it = (*it_predicate)->getTypesList()->at(total+j).begin(); (it != (*it_predicate)->getTypesList()->at(total+j).end()) && !contains; ++it) {
								if ((*it)->getName() == typedList_list->at(i)->getTypes()->at(k)) {
									contains = true;
								}
							}
						}
						if (!contains) {
							same = false;
						}
					}
					total += typedList_list->at(i)->getList()->size();
				}
				
				if (same) {
					return true;
				}
			}
		}
	}
	
	return false;
}

bool Data::addFunctions(vector< pair< string*, vector<TypedList*>* >* > * function_skeleton_list, vector<string> * return_type) {
	bool success = true;
	vector<Type*> return_t = vector<Type*> ();
	
	for (vector<string>::reverse_iterator it_type = return_type->rbegin(); it_type != return_type->rend(); ++it_type) {
		if (!isType(*it_type)) {
			lexical_error("The " + (*it_type) + " type don't exist");
			return false;
		}
		return_t.push_back(getType(*it_type));
	}
	
	for (vector< pair< string*, std::vector<TypedList*>* >* >::reverse_iterator it = function_skeleton_list->rbegin(); it != function_skeleton_list->rend(); ++it) {
		if (!addFunction((*it)->first,  return_t, (*it)->second)) {
			success = false;
		}
	}
	
	return success;
}

bool Data::addFunction(string * name, vector<Type*> return_type, vector<TypedList*> * typedList_list) {
	// the parser is recursive so we must reverse in order to have the arguments of the function in the same order than in the file
	if (isFunction(name, typedList_list)) {
		lexical_error("The " + (*name) + " function already exists with the same types");
		return false;
	}
	
	vector< vector<Type*> > types_list = vector< vector<Type*> > ();
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> ();
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) {
				lexical_error("The " + (*it_type) + " type don't exist");
				return false;
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_variable = (*it)->getList()->rbegin(); it_variable != (*it)->getList()->rend(); ++it_variable) {
			types_list.push_back(types);
		}
	}
	m_functions.push_back(new Function(*name, return_type, types_list));
	return true;
}

bool Data::isFunction(string * name, vector<TypedList*> * typedList_list) {
	unsigned int nb_parameters = 0;
	
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		nb_parameters += (*it)->getList()->size();
	}
	
	for (vector<Function*>::iterator it_function = m_functions.begin(); it_function != m_functions.end(); ++it_function) {
		if ((*name) == (*it_function)->getName()) {
			if (nb_parameters == 0) {
				return true;
			}
			
			if (nb_parameters == (*it_function)->getTypesList()->size()) {
				bool same = true;
				unsigned int total = 0;
				for (unsigned int i=typedList_list->size()-1; (i >= 0) && (total < nb_parameters) && same; --i) {
					for (unsigned int j=0; (j < typedList_list->at(i)->getList()->size()) && same; ++j) {
						bool contains = false;
						for (unsigned int k=0; (k < typedList_list->at(i)->getTypes()->size()) && same && !contains; ++k) {
							for (vector<Type*>::iterator it = (*it_function)->getTypesList()->at(total+j).begin(); (it != (*it_function)->getTypesList()->at(total+j).end()) && !contains; ++it) {
								if ((*it)->getName() == typedList_list->at(i)->getTypes()->at(k)) {
									contains = true;
								}
							}
						}
						if (!contains) {
							same = false;
						}
					}
					total += typedList_list->at(i)->getList()->size();
				}
				
				if (same) {
					return true;
				}
			}
		}
	}
	
	return false;
}

void Data::addProblem(string * name) {
	m_problem = Problem(*name, m_domain);
}

bool Data::addObjects(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> ();
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) {
				lexical_error("The " + (*it_type) + " type don't exist");
				return false;
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_object = (*it)->getList()->rbegin(); it_object != (*it)->getList()->rend(); ++it_object) {
			if (isObject(*it_object)) {
				lexical_error("The " + (*it_object) + " object already exists");
				return false;
			}
			m_object_list.push_back(*it_object);
			m_objects.push_back(new Object(*it_object, types));
		}
	}
	return true;
}

bool Data::isObject(string object) {
	return (find(m_object_list.begin(), m_object_list.end(), object) != m_object_list.end());
}

void Data::display() {
	cout << "Domain : " << m_domain->getName() << endl;
	
	if (m_types.size() != 0) {
		cout << "Types : " << endl;
		for(vector<Type*>::iterator it = m_types.begin(); it != m_types.end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_constants.size() != 0) {
		cout << "Constants : " << endl;
		for(vector<Constant*>::iterator it = m_constants.begin(); it != m_constants.end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_predicates.size() != 0) {
		cout << "Predicates : " << endl;
		for(vector<Predicate*>::iterator it = m_predicates.begin(); it != m_predicates.end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_functions.size() != 0) {
		cout << "Functions : " << endl;
		for(vector<Function*>::iterator it = m_functions.begin(); it != m_functions.end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	cout << "Problem : " << m_problem.getName() << endl;
	
	if (m_objects.size() != 0) {
		cout << "Objects : " << endl;
		for(vector<Object*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_errors.size() != 0) {
		cout << "Errors : " << endl;
		for(vector<string>::iterator it = m_errors.begin(); it != m_errors.end(); ++it)
			cout << "\t" << (*it) << endl;
	}
	
	if (m_actions.size() != 0) {
		cout << "Actions : " << endl;
		for(vector<DurativeAction*>::iterator it = m_actions.begin(); it != m_actions.end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
}

void Data::lexical_error(string msg) {
	if (find(m_errors.begin(), m_errors.end(), msg) == m_errors.end()) {
		m_errors.push_back(msg);
	}
}

void Data::fatal_error(string msg) {
	cerr << "ERROR : " << msg << endl;
	exit(1);
}

Domain * Data::getDomain(){
	return m_domain;
}

//duratives-actions functions

bool Data::isAction(string const * name){
	return (find(m_list_name_action.begin(), m_list_name_action.end(), *name) !=  m_list_name_action.end());
}


bool Data::isPredicate(string * name,vector< vector<Type*> > types){
	for (vector<Predicate*>::iterator it_predicate = m_predicates.begin(); it_predicate != m_predicates.end(); ++it_predicate) {
		if ((*name) == ((*it_predicate)->getName()) && ( (*it_predicate)->getTypesList()->size() == types.size())){
			return true;
		}
	}
	return false;
}

Predicate * Data::getPredicate(string * name,vector< vector<Type*> > types){
	for (vector<Predicate*>::iterator it_predicate = m_predicates.begin(); it_predicate != m_predicates.end(); ++it_predicate) {
		if (((*name) == (*it_predicate)->getName() ) && ( (*it_predicate)->getTypesList()->size() == types.size())){
			return (*it_predicate);
		}
	}
	return new Predicate("inexistant");
}

DurativeAction * Data::addDurationAction(string * name,vector<TypedList*> * typedList_list,float durative,vector< pair< pair< vector< string > *, string *> * ,int** >* > * nearly_conds, vector< pair< pair< vector< string > *, string *> * ,int** >* > * nearly_effects){

	DurativeAction * action; 
	vector<Type *> types;
	vector<Variable *> variable_list;
	vector< vector<Type*> > type_list;
	Fluent * fluent;
	Attribute  att;
	//action name
	if (isAction(name)){
			lexical_error("The " + *name + " action already exists with the same name");
	} else { 
		action = new DurativeAction(*name);
		//action parameters
		// for each vairiable-type , we need to check if type existed and add each variable with his type(s))
		for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it){
			types=  vector<Type *> ();				
			for (vector<string>::iterator it2 = (*it)->getTypes()->begin(); it2 != (*it)->getTypes()->end(); ++it2){
				 if (! isType(*it2)){
					lexical_error("The type " + (*it2) +" doesn't exist");
				}
				types.push_back(getType(*it2));
			}
			for (vector<string>::iterator it2 =(*it)->getList()->begin(); it2 != (*it)->getList()->end();++it2){					
				if (  action->isVariable(*it2)){
					lexical_error("In action " + action->getName() + "The " + (*it2) + " variable already exist");
				}	
				action->addParameters( new Variable(*it2,types) );					
			}
		}
		// action time
		action->addDuration(durative);
		//action conditions 
		for(vector< pair< pair<vector<string> *,string *> * ,int**> * >::iterator it = nearly_conds->begin(); it != nearly_conds->end(); ++it){
				
			// each variable need to be define in parameters
					
			type_list = vector< vector<Type*> >();
			for (vector<string>::iterator it2 =(*it)->first->first->begin(); it2 != (*it)->first->first->end(); ++it2){
				if( !(action->isVariable(*it2)) ){
					lexical_error("In action " + action->getName() + "The variable " + (*it2) + " don't exist");
				}
				type_list.push_back(*(action->getVariable(*it2)->getTypes()));
				variable_list.push_back(action->getVariable(*it2));	
			}
			// predicate existence verification 
			if(!(isPredicate((*it)->first->second,type_list))){
				lexical_error(("In action " + action->getName() + ", the predicate  "+ *(*it)->first->second +"  don't exist").c_str());
			}
			fluent = new Fluent (getPredicate((*it)->first->second,type_list));
			
			
			att =  Attribute();
			switch(*(*it)->second[0]){
				case 0: // at start
					att.addSupported(Interval(0.,0.));
					break;
				case 1:	// at end
					att.addSupported(Interval(durative,durative));
					break;
				case 2: // over all
					att.addSupported(Interval(0.,durative));
					break;
				default:perror("");
				}
			if  (!*((*it)->second[1])){
				action->addCondition(att,fluent);
			} else {
				action->addNotCondition(att,fluent);
			}
		}
		// effects

		for(vector< pair< pair<vector<string> *,string *> * ,int**> * >::iterator it = nearly_effects->begin(); it != nearly_effects->end(); ++it){
			// each variable need to be define in parameters
					
			type_list = vector< vector<Type*> >();
			for (vector<string>::iterator it2 =(*it)->first->first->begin(); it2 != (*it)->first->first->end(); ++it2){
				if( !(action->isVariable(*it2)) ){
					lexical_error("In action " + action->getName() + ", The variable" + (*it2) + " don't exist");
				}
				type_list.push_back(*(action->getVariable(*it2)->getTypes()));
				variable_list.push_back(action->getVariable(*it2));	
			}
			// predicate existence verification 
			if(!(isPredicate((*it)->first->second,type_list))){
				lexical_error("In action " + action->getName() + ", The action  "+ *(*it)->first->second + " don't exist");
			}
			fluent = new Fluent (getPredicate((*it)->first->second,type_list));
			
			att =  Attribute();
			switch(*(*it)->second[0]){
				case 0: // at start
					att.addSupported(Interval(0.,0.));
					break;
				case 1:	// at end
					att.addSupported(Interval(durative,durative));
					break;
				case 2: // over all
					att.addSupported(Interval(0.,durative));
					break;
				default:perror("");
				}
			if  (!*((*it)->second[1])){
				action->addEffect(att,fluent);
			} else {
				action->addNotEffect(att,fluent);
			}
		}
	m_list_name_action.push_back(action->getName());
	m_actions.push_back(action);
	return action;
	}
	return new DurativeAction("inxistant");
}










