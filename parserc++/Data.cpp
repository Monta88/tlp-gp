#include "Data.h"

#include <algorithm>
#include <iostream>

Data::Data() {
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
	Domain * domain = new Domain(m_domain);
	domain->addTypes(m_types);
	domain->addConstants(m_constants);
	domain->addPredicates(m_predicates);
	domain->addFunctions(m_functions);
	domain->addActions(m_actions);
	
	return domain;
}

Problem * Data::getProblem() {
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
	if (isRequirement(req)) {
		return false;
	}
	m_requirements.push_back(req);
	return true;
}

bool Data::isRequirement(int req) {
	return (find(m_requirements.begin(), m_requirements.end(), req) != m_requirements.end());
}

bool Data::addTypes(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists
	vector<Type*> object = vector<Type*>();
	object.push_back(getType("Object"));
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> parents = vector<Type*> ();
		for (vector<string>::reverse_iterator it_parent = (*it)->getTypes()->rbegin(); it_parent != (*it)->getTypes()->rend(); ++it_parent) {
			if (!isType(*it_parent)) {
				m_type_list.push_back(*it_parent);
				m_types->push_back(new Type(*it_parent,object));
			}
			parents.push_back(getType(*it_parent));
		}
		for (vector<string>::reverse_iterator it_type = (*it)->getList()->rbegin(); it_type != (*it)->getList()->rend(); ++it_type) {
			if (isType(*it_type)) {
				//lexical_error("The " + (*it_type) + " type already exists");
				//return false;
			}
			m_type_list.push_back(*it_type);
			m_types->push_back(new Type(*it_type, parents));
		}
	}
	return true;
}

bool Data::isType(string type) {
	return (find(m_type_list.begin(), m_type_list.end(), type) != m_type_list.end());
}

Type * Data::getType(string name) {
	for (vector<Type*>::iterator it = m_types->begin(); it != m_types->end(); ++it) {
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
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type don't exist");
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_constant = (*it)->getList()->rbegin(); it_constant != (*it)->getList()->rend(); ++it_constant) {
			if (isConstant(*it_constant)) {
				getConstant(*it_constant)->addTypes(types);
				lexical_error("The " + (*it_constant) + " constant already exists");
			}
			else {
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
	for (vector<Constant*>::iterator it = m_constants->begin(); it != m_constants->end(); ++it) {
		if ((*it)->getName() == constant) {
			return *it;
		}
	}
	
	fatal_error("Constant \""+ constant +"\" not found");
	
	return NULL;
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
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type don't exist");
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_variable = (*it)->getList()->rbegin(); it_variable != (*it)->getList()->rend(); ++it_variable) {
			types_list.push_back(types);
		}
	}
	m_predicates->push_back(new Predicate(*name, types_list));
	return true;
}

bool Data::isPredicate(string * name, vector<TypedList*> * typedList_list) {
	unsigned int nb_parameters = 0;
	
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		nb_parameters += (*it)->getList()->size();
	}
	
	for (vector<Predicate*>::iterator it_predicate = m_predicates->begin(); it_predicate != m_predicates->end(); ++it_predicate) {
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
			m_type_list.push_back(*it_type);
			m_types->push_back(new Type(*it_type));
			lexical_error("The " + (*it_type) + " type don't exist");
		}
		return_t.push_back(getType(*it_type));
	}
	
	for (vector< pair< string*, vector<TypedList*>* >* >::reverse_iterator it = function_skeleton_list->rbegin(); it != function_skeleton_list->rend(); ++it) {
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
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type don't exist");
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_variable = (*it)->getList()->rbegin(); it_variable != (*it)->getList()->rend(); ++it_variable) {
			types_list.push_back(types);
		}
	}
	m_functions->push_back(new Function(*name, return_type, types_list));
	return true;
}

bool Data::isFunction(string * name, vector<TypedList*> * typedList_list) {
	unsigned int nb_parameters = 0;
	
	for (vector<TypedList*>::iterator it = typedList_list->begin(); it != typedList_list->end(); ++it) {
		nb_parameters += (*it)->getList()->size();
	}
	
	for (vector<Function*>::iterator it_function = m_functions->begin(); it_function != m_functions->end(); ++it_function) {
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
	m_problem = *name;
}

bool Data::addObjects(vector<TypedList*> * typedList_list) {
	// the parser is recursive so if we want the same order than in the file we need to reverse the lists
	for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it) {
		vector<Type*> types = vector<Type*> ();
		for (vector<string>::reverse_iterator it_type = (*it)->getTypes()->rbegin(); it_type != (*it)->getTypes()->rend(); ++it_type) {
			if (!isType(*it_type)) {
				m_type_list.push_back(*it_type);
				m_types->push_back(new Type(*it_type));
				lexical_error("The " + (*it_type) + " type don't exist");
			}
			types.push_back(getType(*it_type));
		}
		for (vector<string>::reverse_iterator it_object = (*it)->getList()->rbegin(); it_object != (*it)->getList()->rend(); ++it_object) {
			if (isObject(*it_object)) {
				getObject(*it_object)->addTypes(types);
			} else {
				m_object_list.push_back(*it_object);
				m_objects->push_back(new Object(*it_object, types));
			}
		}
	}
	return true;
}



bool Data::isObject(string object) {
	if (find(m_constant_list.begin(), m_constant_list.end(), object) != m_constant_list.end()) {
		lexical_error(object + " cannot be an Object because it is already a Constant !");
		return true;
	}
	return (find(m_object_list.begin(), m_object_list.end(), object) != m_object_list.end());
}

Object * Data::getObject(string object) {
	for (vector<Object*>::iterator it = m_objects->begin(); it != m_objects->end(); ++it) {
		if ((*it)->getName() == object) {
			return *it;
		}
	}
	
	fatal_error("Object \""+ object +"\" not found");
	
	return NULL;
}

bool Data::addInit(pair< pair< vector< string > *, string *> *, bool > * literal, float at) {
	vector<vector<Type*> > type_list = vector<vector<Type*> > ();
	vector<Member *> members_list = vector<Member *> ();
	Member * member;
	Predicate * predicate;
	Attribute attribute;
	
	for (vector<string>::reverse_iterator it_member = literal->first->first->rbegin(); it_member != literal->first->first->rend(); ++it_member) {
		if (isConstant(*it_member)) {
			member = getConstant(*it_member);
		}
		else {
			if (isObject(*it_member)) {
				member = getObject(*it_member);
			}
			else {
				lexical_error((*it_member) + " is nor a Constant nor an Object");
				return false;
			}
		}
		members_list.push_back(member);
		type_list.push_back(*(member->getTypes()));
	}
	
	if (isPredicate(literal->first->second, type_list)) {
		predicate = getPredicate(literal->first->second, type_list);
	}
	else {
		fatal_error("Predicate \""+ *(literal->first->second) +"\" not found");
	}
	
	attribute = Attribute();
	if (literal->second) {
		attribute.addSupported(Interval(at, at));
	}
	else {
		attribute.addNotSupported(Interval(at, at));
	}
	
	m_inits->push_back(make_pair(new Fluent(predicate, members_list), attribute));
	
	return true;
}

bool Data::addGoals(vector< vector< pair< pair< vector< string > *, string *> *, int >* > * > * pre_GD) { 
	vector<Member *> members_list = vector<Member *> ();
	Member * member;
	vector< vector<Type*> > type_list;
	Predicate * predicate;
	Attribute  att;
	
	for (vector< vector< pair< pair< vector< string > *, string *> *, int >* > * >::reverse_iterator it_main = pre_GD->rbegin(); it_main != pre_GD->rend(); it_main++) {
		for(vector< pair< pair<vector<string> *,string *> *, int> * >::reverse_iterator it = (*it_main)->rbegin(); it != (*it_main)->rend(); ++it){			
			members_list = vector<Member *> ();
			type_list = vector< vector<Type*> >();
			for (vector<string>::reverse_iterator it_member =(*it)->first->first->rbegin(); it_member != (*it)->first->first->rend(); ++it_member){
				if (isConstant(*it_member)) {
					member = getConstant(*it_member);
				}
				else {
					if (isObject(*it_member)) {
						member = getObject(*it_member);
					}
					else {
						lexical_error((*it_member) + " is nor a Constant nor an Object");
						return false;
					}
				}
				members_list.push_back(member);
				type_list.push_back(*(member->getTypes()));
			}
			
			if (isPredicate((*it)->first->second, type_list)) {
				predicate = getPredicate((*it)->first->second, type_list);
			}
			else {
				fatal_error("Predicate \""+ *(*it)->first->second +"\" not found");
				return false;
			}
			
			att = Attribute();
			if  (!((*it)->second & 0b1)){
				att.addSupported(Interval(0., 0.));
			} else {
				att.addNotSupported(Interval(0., 0.));
			}
			
			m_goals->push_back(make_pair(new Fluent(predicate, members_list), att));
		}
	}
	
	return true;
}

vector<pair<Fluent*, Attribute> > * Data::getGoals() {
	return m_goals;
}

void Data::display() {
	cout << "Domain : " << m_domain << endl;
	
	if (m_types->size() != 0) {
		cout << "Types : " << endl;
		for(vector<Type*>::iterator it = m_types->begin(); it != m_types->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_constants->size() != 0) {
		cout << "Constants : " << endl;
		for(vector<Constant*>::iterator it = m_constants->begin(); it != m_constants->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_predicates->size() != 0) {
		cout << "Predicates : " << endl;
		for(vector<Predicate*>::iterator it = m_predicates->begin(); it != m_predicates->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_functions->size() != 0) {
		cout << "Functions : " << endl;
		for(vector<Function*>::iterator it = m_functions->begin(); it != m_functions->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	cout << "Problem : " << m_problem << endl;
	
	if (m_objects->size() != 0) {
		cout << "Objects : " << endl;
		for(vector<Object*>::iterator it = m_objects->begin(); it != m_objects->end(); ++it)
			cout << "\t" << (*it)->to_string() << endl;
	}
	
	if (m_inits->size() != 0) {
		cout << "Inits : " << endl;
		for(vector<pair<Fluent*, Attribute> >::iterator it = m_inits->begin(); it != m_inits->end(); ++it)
			cout << "\t" << (*it).first->to_string() << " <-> " << (*it).second.to_string() << endl;
	}
	
	if (m_goals->size() != 0) {
		cout << "Goals : " << endl;
		for(vector<pair<Fluent*, Attribute> >::iterator it = m_goals->begin(); it != m_goals->end(); ++it)
			cout << "\t" << (*it).first->to_string() << " <-> " << (*it).second.to_string() << endl;
	}
	
	if (m_errors.size() != 0) {
		cout << "Errors : " << endl;
		for(vector<string>::iterator it = m_errors.begin(); it != m_errors.end(); ++it)
			cout << "\t" << (*it) << endl;
	}
	
	if (m_actions->size() != 0) {
		cout << "Actions :" << endl;
		for(vector<DurativeAction*>::iterator it = m_actions->begin(); it != m_actions->end(); ++it)
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

//numbers-actions functions

bool Data::isAction(string const * name){
	return (find(m_list_name_action.begin(), m_list_name_action.end(), *name) !=  m_list_name_action.end());
}

bool Data::isPredicate(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0;
	
	nb_parameters = types.size();
	
	for (vector<Predicate*>::iterator it_predicate = m_predicates->begin(); it_predicate != m_predicates->end(); ++it_predicate) {
		if ((*name) == (*it_predicate)->getName()) {
			if (nb_parameters == 0) {
				return true;
			}
			
			if (nb_parameters == (*it_predicate)->getTypesList()->size()) {
				bool same = true;
				
				for (unsigned int i=0; (i < types.size()) && same; ++i) {
					bool contains = false;
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) {
						for (vector<Type*>::iterator it = (*it_predicate)->getTypesList()->at(i).begin(); (it != (*it_predicate)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) {
								contains = true;
							}
						}
					}
					if (!contains) {
						same = false;
					}
				}
				
				if (same) {
					return true;
				}
			}
		}
	}
	
	return false;
}

Predicate * Data::getPredicate(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0;
	
	nb_parameters = types.size();
	
	for (vector<Predicate*>::iterator it_predicate = m_predicates->begin(); it_predicate != m_predicates->end(); ++it_predicate) {
		if ((*name) == (*it_predicate)->getName()) {
			if (nb_parameters == 0) {
				return *it_predicate;
			}
			
			if (nb_parameters == (*it_predicate)->getTypesList()->size()) {
				bool same = true;
				
				for (unsigned int i=0; (i < types.size()) && same; ++i) {
					bool contains = false;
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) {
						for (vector<Type*>::iterator it = (*it_predicate)->getTypesList()->at(i).begin(); (it != (*it_predicate)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) {
								contains = true;
							}
						}
					}
					if (!contains) {
						same = false;
					}
				}
				
				if (same) {
					return *it_predicate;
				}
			}
		}
	}
	
	fatal_error("You tried to get the predicate \""+ *name +"\" but couldn't find it");
	
	return NULL;
}

bool Data::addDurativeAction(string * name, vector<TypedList*> * typedList_list, float number, vector< pair< pair< vector< string > *, string *> *, int >* > * timed_GD, vector< pair< pair< vector< string > *, string *> *, int >* > * cond_effect){

	DurativeAction * action; 
	vector<Type *> types;
	vector<Member *> variable_list =  vector<Member *>();
	vector< vector<Type*> > type_list;
	Fluent * fluent;
	Attribute  att;
	//action name
	if (isAction(name)){
			lexical_error("The " + *name + " action already exists with the same name");
			return false;
	} else { 
		action = new DurativeAction(*name);
		//action parameters
		// for each vairiable-type , we need to check if type existed and add each variable with his type(s))
		for (vector<TypedList*>::reverse_iterator it = typedList_list->rbegin(); it != typedList_list->rend(); ++it){
			types=  vector<Type *> ();				
			for (vector<string>::reverse_iterator it2 = (*it)->getTypes()->rbegin(); it2 != (*it)->getTypes()->rend(); ++it2){
				 if (! isType(*it2)) {
					lexical_error("The type " + (*it2) +" doesn't exist");
					return false;
				}
				types.push_back(getType(*it2));
			}
			for (vector<string>::reverse_iterator it2 =(*it)->getList()->rbegin(); it2 != (*it)->getList()->rend();++it2){					
				if (  action->isVariable(*it2)){
					lexical_error("In action " + action->getName() + "The " + (*it2) + " variable already exist");
					return false;
				}	
				action->addParameters( Variable(*it2,types) );					
			}
		}
		// action time
		action->addDuration(number);
		//action conditions 
		for(vector< pair< pair<vector<string> *,string *> * , int> * >::reverse_iterator it = timed_GD->rbegin(); it != timed_GD->rend(); ++it){
				
			// each variable need to be define in parameters
			variable_list =  vector<Member *>();
			type_list = vector< vector<Type*> >();
			for (vector<string>::reverse_iterator it2 =(*it)->first->first->rbegin(); it2 != (*it)->first->first->rend(); ++it2){
				if( (action->isVariable(*it2)) ){
					type_list.push_back(*(action->getVariable(*it2)->getTypes()));
					variable_list.push_back(action->getVariable(*it2));
				} else if (isConstant(*it2)) {
					type_list.push_back(*(getConstant(*it2)->getTypes()));
					variable_list.push_back(getConstant(*it2));
				} else {
					lexical_error("In action " + action->getName() + " the variable " + (*it2) + " don't exist");
					return false;
				}	
			}
			// predicate existence verification 
			if(!(isPredicate((*it)->first->second,type_list))){
				lexical_error(("In action " + action->getName() + ", the predicate  "+ *(*it)->first->second +"  don't exist").c_str());
				return false;
			}else {
				fluent = new Fluent (getPredicate((*it)->first->second, type_list),variable_list);
			}
			
			att =  Attribute();
			switch(((*it)->second & 0b1110) >> 1){
				case 0b001: 	// at start
					att.addSupported(Interval(0.,0.));
					break;
				case 0b0000010:		// at end
					att.addSupported(Interval(number,number));
					break;
				case 0b0000100: 	// over all
					att.addSupported(Interval(0.,number));
					break;
				case 0b0001000: 	// supported all
					att.addSupported(Interval(0.,number));
					break;
				case 0b0010000: 	// forbidden all
					att.addForbidden(Interval(0.,number));
					break;
				case 0b0100000: 	// somewhere all
					att.addSupported(Interval(0.,0.)); // wrong translation
					lexical_error("Somwhere not supported");
					break;
				case 0b1000000: 	// anywhere all
					att.addSupported(Interval(0.,0.)); // wrong translation
					lexical_error("Anywhere not supported");
					break;
				default:
					lexical_error("Something went wrong while reading conditions");
					return false;
				}
			if  (!((*it)->second & 0b1)){
				action->addCondition(att,fluent);
			} else {
				action->addNotCondition(att,fluent);
			}
			variable_list.clear();
		}
		// effects
		
		for(vector< pair< pair<vector<string> *,string *> *, int> * >::reverse_iterator it = cond_effect->rbegin(); it != cond_effect->rend(); ++it){
			// each variable need to be define in parameters
					
			type_list = vector< vector<Type*> >();
			for (vector<string>::reverse_iterator it2 =(*it)->first->first->rbegin(); it2 != (*it)->first->first->rend(); ++it2){
				if( (action->isVariable(*it2)) ){
					type_list.push_back(*(action->getVariable(*it2)->getTypes()));
					variable_list.push_back(action->getVariable(*it2));
				} else if (isConstant(*it2)) {
					type_list.push_back(*(getConstant(*it2)->getTypes()));
					variable_list.push_back(getConstant(*it2));
				} else {
					lexical_error("In action " + action->getName() + " the variable " + (*it2) + " don't exist");
					return false;
				}	
			}
			// predicate existence verification 
			if(!(isPredicate((*it)->first->second,type_list))){
				lexical_error("In action " + action->getName() + ", The action  "+ *(*it)->first->second + " don't exist");
				return false;
			} else {
				fluent = new Fluent (getPredicate((*it)->first->second, type_list),variable_list);
			}
			
			att =  Attribute();
			switch(((*it)->second & 0b1110) >> 1){
				case 0b001: 	// at start
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
				case 0b000100000:		// somewhere all
					att.addSupported(Interval(0.,0.)); // wrong translation
					lexical_error("Somwhere not supported");
					break;
				case 0b001000000:		// anywhere all
					att.addSupported(Interval(0.,0.)); // wrong translation
					lexical_error("Anywhere not supported");
					break;
				case 0b100000000:		// ->over all
					att.addSupported(Interval(number,number));
					att.addForbidden(Interval(0.,number));
					att.addNotForbidden(Interval(0.,number, false, false));
					break;
				default:
					lexical_error("Something went wrong while reading effects");
					return false;
				}
			if  (!((*it)->second & 0b1)){
				action->addEffect(att,fluent);
			} else {
				action->addNotEffect(att,fluent);
			}
		variable_list.clear();
		}
		m_list_name_action.push_back(action->getName());
		m_actions->push_back(action);
	}
	return true;
}

vector<pair<Fluent*, Attribute> > * Data::getInits(){
	return m_inits;
}

vector<DurativeAction*> * Data::getActions(){
	return m_actions;
}

bool Data::addInitiatedFunction(pair< vector< string > *, string *> * atomic_formula, float number) {
	vector<vector<Type*> > type_list = vector<vector<Type*> > ();
	vector<Member *> members_list = vector<Member *> ();
	Member * member;
	Function * function;
	
	for (vector<string>::reverse_iterator it_member = atomic_formula->first->rbegin(); it_member != atomic_formula->first->rend(); ++it_member) {
		if (isConstant(*it_member)) {
			member = getConstant(*it_member);
		}
		else {
			if (isObject(*it_member)) {
				member = getObject(*it_member);
			}
			else {
				lexical_error((*it_member) + " is nor a Constant nor an Object");
				return false;
			}
		}
		members_list.push_back(member);
		type_list.push_back(*(member->getTypes()));
	}
	
	if (isFunction(atomic_formula->second, type_list)) {
		function = getFunction(atomic_formula->second, type_list);
	}
	else {
		fatal_error("Function \""+ *(atomic_formula->second) +"\" not found");
	}
	
	m_initiated_functions.push_back(make_pair(make_pair(function, members_list), number));
	
	return true;
}

bool Data::isFunction(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0;
	
	nb_parameters = types.size();
	
	for (vector<Function*>::iterator it_function = m_functions->begin(); it_function != m_functions->end(); ++it_function) {
		if ((*name) == (*it_function)->getName()) {
			if (nb_parameters == 0) {
				return true;
			}
			
			if (nb_parameters == (*it_function)->getTypesList()->size()) {
				bool same = true;
				
				for (unsigned int i=0; (i < types.size()) && same; ++i) {
					bool contains = false;
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) {
						for (vector<Type*>::iterator it = (*it_function)->getTypesList()->at(i).begin(); (it != (*it_function)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) {
								contains = true;
							}
						}
					}
					if (!contains) {
						same = false;
					}
				}
				
				if (same) {
					return true;
				}
			}
		}
	}
	
	return false;
}

Function * Data::getFunction(string * name,vector< vector<Type*> > types) {
	unsigned int nb_parameters = 0;
	
	nb_parameters = types.size();
	
	for (vector<Function*>::iterator it_function = m_functions->begin(); it_function != m_functions->end(); ++it_function) {
		if ((*name) == (*it_function)->getName()) {
			if (nb_parameters == 0) {
				return (*it_function);
			}
			
			if (nb_parameters == (*it_function)->getTypesList()->size()) {
				bool same = true;
				
				for (unsigned int i=0; (i < types.size()) && same; ++i) {
					bool contains = false;
					for (unsigned int k=0; (k < types.at(i).size()) && same && !contains; ++k) {
						for (vector<Type*>::iterator it = (*it_function)->getTypesList()->at(i).begin(); (it != (*it_function)->getTypesList()->at(i).end()) && !contains; ++it) {
							if (((*it)->getName() == types.at(i).at(k)->getName())||(types.at(i).at(k)->isOneOfParents((*it)->getName()))) {
								contains = true;
							}
						}
					}
					if (!contains) {
						same = false;
					}
				}
				
				if (same) {
					return (*it_function);
				}
			}
		}
	}
	
	fatal_error("You tried to get the function \""+ *name +"\" but couldn't find it");
	
	return NULL;
}

float Data::getFunctionReturn(string * name, vector<string> * list_term) {
	unsigned int nb_parameters = 0;
	
	nb_parameters = list_term->size();
	
	for (vector< pair< pair<Function*, vector<Member*> >, float> >::iterator it_initiated_function = m_initiated_functions.begin(); it_initiated_function != m_initiated_functions.end(); ++it_initiated_function) {
		if ((*name) == (*it_initiated_function).first.first->getName()) {
			if (nb_parameters == 0) {
				return (*it_initiated_function).second;
			}
			
			if (nb_parameters == (*it_initiated_function).first.second.size()) {
				bool same = true;
				
				for (unsigned int i=0; (i < list_term->size()) && same; ++i) {
					if (list_term->at(i) != (*it_initiated_function).first.second.at(i)->getName()) {
						same = false;
					}
				}
				
				if (same) {
					return (*it_initiated_function).second;
				}
			}
		}
	}
	
	fatal_error("You tried to get the return of \""+ *name +"\" function but couldn't find it");
	
	return -1.;
}

